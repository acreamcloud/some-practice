// 一个简单的3D射击游戏框架
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// 定义一个三维向量类
class Vector3 {
public:
    double x, y, z; // 向量的三个分量
    // 构造函数
    Vector3(double x = 0, double y = 0, double z = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    // 向量加法
    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    // 向量减法
    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    // 向量数乘
    Vector3 operator*(double k) const {
        return Vector3(x * k, y * k, z * k);
    }
    // 向量点积
    double dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    // 向量叉积
    Vector3 cross(const Vector3& v) const {
        return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    // 向量长度
    double length() const {
        return sqrt(dot(*this));
    }
    // 向量归一化
    void normalize() {
        double len = length();
        if (len > 0) {
            x /= len;
            y /= len;
            z /= len;
        }
    }
};
const double GRAVITY_CONSTANT = -9.8; // 重力常数，向下为负方向
Vector3 gravity(0, GRAVITY_CONSTANT, 0); // 定义重力向量

// 定义一个物体类
class Object {
public:
    Vector3 position; // 物体的位置
    Vector3 velocity; // 物体的速度
    double mass; // 物体的质量
    // 构造函数
    Object(Vector3 position = Vector3(), Vector3 velocity = Vector3(), double mass = 1) {
        this->position = position;
        this->velocity = velocity;
        this->mass = mass;
    }
    // 物体的运动更新函数，根据物理定律计算新的位置和速度
    void update(double dt) {
        position = position + velocity * dt; // 位置更新公式：x' = x + vt
        velocity = velocity + gravity * dt; // 速度更新公式：v' = v + at，其中a是重力加速度
    }
};

// 定义一个子弹类，继承自物体类
class Bullet : public Object {
public:
    double damage; // 子弹的伤害值
    bool active; // 子弹是否有效，如果超出范围或击中目标则无效
    // 构造函数
    Bullet(Vector3 position = Vector3(), Vector3 velocity = Vector3(), double mass = 0.01, double damage = 10) : Object(position, velocity, mass) {
        this->damage = damage;
        this->active = true;
    }
};

// 定义一个敌人类，继承自物体类
class Enemy : public Object {
public:
    double health; // 敌人的生命值
    bool alive; // 敌人是否存活，如果生命值为零则死亡
    // 构造函数
    Enemy(Vector3 position = Vector3(), Vector3 velocity = Vector3(), double mass = 1, double health = 100) : Object(position, velocity, mass) {
        this->health = health;
        this->alive = true;
    }
};

// 定义一个玩家类，继承自物体类
class Player : public Object {
public:
    double health; // 玩家的生命值
    bool alive; // 玩家是否存活，如果生命值为零则死亡
    vector<Bullet> bullets; // 玩家发射的子弹列表
    int score; // 玩家的得分，击杀敌人可以增加得分
    // 构造函数
    Player(Vector3 position = Vector3(), Vector3 velocity = Vector3(), double mass = 1, double health = 100) : Object(position, velocity, mass) {
        this->health = health;
        this->alive = true;
        this->score = 0;
    }
};

// 定义一些全局变量和常量

const int MAX_BULLETS = 100; // 最大子弹数量，超过则删除最早发射的子弹
const int MAX_ENEMIES = 10; // 最大敌人数量，超过则不再生成新的敌人

const double BULLET_SPEED = 50; // 子弹的初始速度


const double WORLD_SIZE_X = 100; // 游戏世界的X轴范围，从-50到50
const double WORLD_SIZE_Y = 100; // 游戏世界的Y轴范围，从-50到50
const double WORLD_SIZE_Z = 100; // 游戏世界的Z轴范围，从-50到50


Player player(Vector3(0, 0, -50)); // 创建玩家对象，初始位置在Z轴负方向

vector<Enemy> enemies; // 创建敌人对象列表

double timeElapsed; // 游戏时间

// 定义一些辅助函数

// 检查两个物体是否相撞，简化为检查两个物体之间的距离是否小于一定阈值（这里设为1）
bool checkCollision(Object& a, Object& b) {
    return (a.position - b.position).length() < 1;
}

// 检查一个物体是否超出游戏世界范围，如果是则返回true，否则返回false
bool checkOutOfBound(Object& o) {
   return o.position.x < -WORLD_SIZE_X / 2 || o.position.x > WORLD_SIZE_X / 2 ||
          o.position.y < -WORLD_SIZE_Y / 2 || o.position.y > WORLD_SIZE_Y / 2 ||
          o.position.z < -WORLD_SIZE_Z / 2 || o.position.z > WORLD_SIZE_Z / 2;
}

// 随机生成一个敌人对象，并添加到敌人列表中，如果敌人数量超过最大值，则不生成新的敌人
void spawnEnemy() {
   if (enemies.size() < MAX_ENEMIES) { 
       Enemy enemy(Vector3(rand() % (int)WORLD_SIZE_X - WORLD_SIZE_X / 2,
                           rand() % (int)WORLD_SIZE_Y - WORLD_SIZE_Y / 2,
                           rand() % (int)WORLD_SIZE_Z - WORLD_SIZE_Z / 2)); 
       enemies.push_back(enemy); 
   } 
}

// 玩家发射一颗子弹，并添加到子弹列表中，如果子弹数量超过最大值，则删除最早发射的子弹。子弹的初始位置和速度由玩家的位置和方向决定（这里简化为Z轴正方向）
void shootBullet() {
   Bullet bullet(player.position + Vector3(0, 0, BULLET_SPEED), 
                 player.velocity + Vector3(0, 0, BULLET_SPEED)); 
   player.bullets.push_back(bullet); 
   if (player.bullets.size() > MAX_BULLETS) { 
       player.bullets.erase(player.bullets.begin()); 
   } 
}
// 处理玩家和敌人之间的碰撞检测和伤害计算。如果玩家和敌人相撞，则玩家和敌人都受到伤害，并且敌人死亡。如果玩家或敌人死亡，则设置相应的状态标志。
void handlePlayerEnemyCollision() {
   for (int i = 0; i < enemies.size(); i++) { // 遍历所有敌人
       if (checkCollision(player, enemies[i])) { // 如果玩家和敌人相撞
           player.health -= enemies[i].health; // 玩家受到敌人生命值的伤害
           enemies[i].health = 0; // 敌人生命值归零
           enemies[i].alive = false; // 敌人死亡
       }
       if (player.health <= 0) { // 如果玩家生命值为零或以下
           player.alive = false; // 玩家死亡
           break; // 结束循环
       }
   }
}

// 处理子弹和敌人之间的碰撞检测和伤害计算。如果子弹和敌人相撞，则子弹无效，并且敌人受到子弹伤害值的伤害。如果敌人死亡，则玩家得分增加，并且设置相应的状态标志。
void handleBulletEnemyCollision() {
   for (int i = 0; i < player.bullets.size(); i++) { // 遍历所有子弹
       for (int j = 0; j < enemies.size(); j++) { // 遍历所有敌人
           if (checkCollision(player.bullets[i], enemies[j])) { // 如果子弹和敌人相撞
               player.bullets[i].active = false; // 子弹无效
               enemies[j].health -= player.bullets[i].damage; // 敌人受到子弹伤害值的伤害
               if (enemies[j].health <= 0) { // 如果敌人生命值为零或以下
                   enemies[j].alive = false; // 敌人死亡
                   player.score += 10; // 玩家得分增加10分
               }
               break; // 结束内层循环
           }
       }
   }
}

// 处理游戏逻辑更新，包括物体的运动更新，碰撞检测，状态判断等。
void updateGameLogic(double dt) {
   player.update(dt); // 更新玩家的位置和速度

   for (int i = 0; i < player.bullets.size(); i++) { // 遍历所有子弹
       player.bullets[i].update(dt); // 更新子弹的位置和速度
       if (checkOutOfBound(player.bullets[i])) { // 如果子弹超出游戏世界范围
           player.bullets[i].active = false; // 子弹无效
       }
   }

   for (int i = 0; i < enemies.size(); i++) { // 遍历所有敌人
       enemies[i].update(dt); // 更新敌人的位置和速度
       if (checkOutOfBound(enemies[i])) { // 如果敌人超出游戏世界范围
           enemies[i].alive = false; // 敌人死亡
       }
   }

   handlePlayerEnemyCollision(); // 处理玩家和敌人之间的碰撞检测和伤害计算

   handleBulletEnemyCollision(); // 处理子弹和敌人之间的碰撞检测和伤害计算

   for (int i = 0; i < player.bullets.size(); i++) { // 遍历所有子弹
       if (!player.bullets[i].active) { // 如果子弹无效
           player.bullets.erase(player.bullets.begin() + i); // 删除该子弹
           i--; // 索引减一，避免跳过下一个元素
       }
   }

   for (int i = 0; i < enemies.size(); i++) { // 遍历所有敌人
       if (!enemies[i].alive) { // 如果敌人死亡
           enemies.erase(enemies.begin() + i); // 删除该敌人
           i--; // 索引减一，避免跳过下一个元素
       }
   }

   timeElapsed += dt; // 游戏时间增加

   if (timeElapsed > 10) { // 每隔10秒生成一个新的敌人
       spawnEnemy(); 
       timeElapsed -= 10;
   }
}

// 定义一些输入输出函数

// 获取用户输入，简化为只有两种输入：发射子弹或退出游戏。如果用户输入S或s，则发射一颗子弹。如果用户输入Q或q，则退出游戏。其他输入则忽略。
void getUserInput() {
   char input;
   cin >> input;
   if (input == 'S' || input == 's') {
       shootBullet();
   } else if (input == 'Q' || input == 'q') {
       exit(0);
   }
}

// 显示游戏画面，简化为只显示玩家，子弹，敌人的位置，以及玩家的生命值和得分。使用简单的符号表示不同的物体：P表示玩家，B表示子弹，E表示敌人。
void showGameScreen() {
   cout << "Game Screen:\n";
   cout << "Player: P (" << player.position.x << ", " << player.position.y << ", " << player.position.z << ")\n";
   cout << "Bullets: ";
   for (int i = 0; i < player.bullets.size(); i++) {
       cout << "B (" << player.bullets[i].position.x << ", " << player.bullets[i].position.y << ", " << player.bullets[i].position.z << ") ";
   }
   cout << "\n";
   cout << "Enemies: ";
   for (int i = 0; i < enemies.size(); i++) {
       cout << "E (" << enemies[i].position.x << ", " << enemies[i].position.y << ", " << enemies[i].position.z << ") ";
   }
   cout << "\n";
   cout << "Player Health: " << player.health << "\n";
   cout << "Player Score: " << player.score << "\n";
}

void restartGame() {
    player.position = Vector3(0, 0, -50);
    player.velocity = Vector3();
    player.health = 100;
    player.alive = true;
    player.bullets.clear();
    player.score = 0;

    enemies.clear();

    timeElapsed = 0;
}
// 显示游戏结束画面，显示玩家的最终得分，并提示用户是否重新开始游戏。如果用户输入Y或y，则重新开始游戏。如果用户输入N或n，则退出游戏。其他输入则忽略。
void showGameOverScreen() {
    cout << "Game Over!\n";
    cout << "Your final score is: " << player.score << "\n";
    cout << "Do you want to play again? (Y/N)\n";
    char input;
    cin >> input;
    if (input == 'Y' || input == 'y') {
        restartGame();
    } else if (input == 'N' || input == 'n') {
        exit(0);
    }
}

// 重新开始游戏，重置所有变量和对象到初始状态。

// 定义主函数，开始游戏循环

int main() {
    srand(time(NULL)); // 设置随机数种子

    double dt = 0.1; // 设置每一帧的时间间隔为0.1秒

    while (true) { // 游戏循环开始

        getUserInput(); // 获取用户输入

        updateGameLogic(dt); // 更新游戏逻辑

        showGameScreen(); // 显示游戏画面

        if (!player.alive) { // 如果玩家死亡，则显示游戏结束画面
            showGameOverScreen();
        }
    }

    return 0; // 游戏循环结束
}