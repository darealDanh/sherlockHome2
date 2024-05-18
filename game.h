/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 01.03.2023
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <climits>
#include <cassert>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class Robot;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;
class Sherlock;
class Watson;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType
{
    MAGIC_BOOK,
    ENERGY_DRINK,
    FIRST_AID,
    EXCEMPTION_CARD,
    PASSING_CARD
};
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};

class MapElement
{
protected:
    ElementType type;

public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement
{
public:
    Path();
};

class Wall : public MapElement
{
public:
    Wall();
};

class FakeWall : public MapElement
{
private:
    int req_exp, r, c;

public:
    FakeWall(int r, int c);
    int getReqExp() const;
};

class Position
{
private:
    int r, c;

public:
    static const Position npos;

    Position(int r = 0, int c = 0);

    Position(const string &str_pos); //(r,c)

    int getRow() const { return r; };
    int getCol() const { return c; };
    void setRow(int r) { this->r = r; };
    void setCol(int c) { this->c = c; };

    string str() const
    {
        return "(" + to_string(r) + "," + to_string(c) + ")";
    };

    bool isEqual(const Position &pos) const;
};

class MovingObject
{
    friend class TestStudyInPink;

protected:
    int index;
    Position pos;
    Map *map;
    string name;
    int hp;
    int exp;

public:
    MovingObject(int index, const Position pos, Map *map, const string &name);
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    virtual Position getCurrentPosition() const = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    static const Position npos;
    string getName() const;
    int getHP() const;
    int getExp() const;
    void setHP(int hp);
    void setExp(int exp);
};

class Map
{
    friend class TestStudyInPink;

private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();
    bool isValid(const Position &pos, MovingObject *mv_obj) const;
    int getNumRow() const { return num_rows; };
    int getNumCol() const { return num_cols; };
};

class Robot : public MovingObject
{
protected:
    RobotType robot_type;
    BaseItem *item;

public:
    Robot(int index, const Position &init_pos, Map *map, RobotType robot_type);
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    void move();
    static int countRobots;
    virtual BaseItem *getItem();
    int getRobotType() const
    {
        return robot_type;
    };
};

class Character : public MovingObject
{
    friend class TestStudyInPink;

protected:
    string moving_rule;
    BaseBag *bag;

public:
    Character(int index, const Position &init_pos, Map *map, const string &name);
    virtual Position getNextPosition();
    virtual Position getCurrentPosition() const;
    virtual void move();
    virtual string str() const = 0;
    BaseBag *getBag() const;
};
class Sherlock /* TODO */ : public Character
{
private:
    Robot *robot;
    RobotC *robotC;
    Criminal *criminal;
    Watson *watson;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    string str() const;
    void meetRobotC(RobotC *robotC, Criminal *criminal);
    void meetRobot(Robot *robot);
    void meetWatson(Watson *watson);
    void meetCriminal(Criminal *criminal);
    void move();
};

class Watson /* TODO */ : public Character
{
private:
    Robot *robot;
    RobotC *robotC;
    Criminal *criminal;
    Sherlock *sherlock;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    string str() const;
    void meetRobotC(RobotC *robotC, Criminal *criminal);
    void meetRobot(Robot *robot);
    void meetSherlock(Sherlock *sherlock);
    void meetCriminal(Criminal *criminal);
    void move();
};

class Criminal /* TODO */ : public Character
{
private:
    Sherlock *sherlock;
    Watson *watson;
    int countSteps;
    Robot *robot;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    string str() const;
    Position getNextPosition();
    void move();
    int getCountSteps() const
    {
        return countSteps;
    };
    Robot *getRobot() const;
};

class RobotC : public Robot
{
private:
    Criminal *criminal;

public:
    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal);
    Position getNextPosition();
    string str() const;
    int getDistance(Character *character) const;
};

class RobotS : public Robot
{
private:
    Sherlock *sherlock;
    Criminal *criminal;

public:
    RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *Sherlock);
    Position getNextPosition();
    string str() const;
    int getDistance() const;
};

class RobotW : public Robot
{
private:
    Watson *watson;
    Criminal *criminal;

public:
    RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson);
    Position getNextPosition();
    string str() const;
    int getDistance() const;
};

class RobotSW : public Robot
{
private:
    Criminal *criminal;
    Sherlock *sherlock;
    Watson *watson;

public:
    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    Position getNextPosition();
    string str() const;
    int getDistance() const;
};

class BaseItem
{
protected:
    ItemType item_type;
    Character *obj;
    Robot *robot;

public:
    int value;
    BaseItem *next;
    BaseItem(ItemType item_type, int value);
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
    ItemType getItemType() const;
};

class MagicBook : public BaseItem
{
public:
    MagicBook();
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
};

class EnergyDrink : public BaseItem
{
public:
    EnergyDrink();
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
};

class FirstAid : public BaseItem
{
public:
    FirstAid();
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
};

class ExemptionCard : public BaseItem
{
public:
    ExemptionCard();
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
};

class PassingCard : public BaseItem
{
private:
    string challenges;

public:
    PassingCard(const string &challenges);
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    string getChallenge();
};

class BaseBag
{
protected:
    BaseItem *head;
    BaseItem *tail;
    Character *character;
    int countItem;

public:
    BaseBag(Character *character);
    ~BaseBag();
    virtual bool insert(BaseItem *item);
    virtual BaseItem *get();
    virtual BaseItem *get(ItemType itemType);
    virtual string str() const;
    int getCountItem() const
    {
        return countItem;
    };
};

class WatsonBag : public BaseBag
{
private:
    Watson *watson;

public:
    WatsonBag(Watson *watson);
};

class SherlockBag : public BaseBag
{
private:
    Sherlock *sherlock;

public:
    SherlockBag(Sherlock *sherlock);
};

class ArrayMovingObject
{
private:
    MovingObject **arr_mv_objs;
    int capacity;
    int count;

public:
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject();
    bool isFull() const;
    bool add(MovingObject *mv_obj);
    MovingObject *get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const;
};

class Configuration
{
    friend class StudyInPinkProgram;

private:
    // TODO
    int map_num_rows, map_num_cols, max_num_moving_objects, num_walls, num_fake_walls, sherlock_init_hp, sherlock_init_exp, watson_init_hp, watson_init_exp, num_steps;
    Position *arr_walls, *arr_fake_walls, *sherlock_init_pos, *watson_init_pos, *criminal_init_pos;
    string sherlock_moving_rule, watson_moving_rule;

public:
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
};

// Robot, BaseItem, BaseBag,...

class StudyInPinkProgram
{
    friend class Configuration;

private:
    // Sample attributes
    Configuration *config;

    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

    Map *map;
    ArrayMovingObject *arr_mv_objs;

public:
    StudyInPinkProgram(const string &config_file_path);

    bool isStop() const;

    void printResult() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Watson caught the criminal" << endl;
        }
        else
        {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const
    {
        cout << "Step: " << setw(4) << setfill('0') << si
             << "--"
             << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose)
    {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        for (int istep = 0; istep < config->num_steps; ++istep)
        {
            for (int i = 0; i < arr_mv_objs->size(); ++i)
            {
                arr_mv_objs->get(i)->move();
                if (isStop())
                {
                    printStep(istep);
                    break;
                }
                if (verbose)
                {
                    printStep(istep);
                }
            }
        }
        printResult();
    }

    ~StudyInPinkProgram();
};