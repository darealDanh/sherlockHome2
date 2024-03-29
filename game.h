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
    MapElement(ElementType in_type)
    {
        this->type = in_type;
    };
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

class Map
{
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
protected:
    int index;
    Position pos;
    Map *map;
    string name;
    int hp;
    int exp;

public:
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    virtual Position getCurrentPosition() const = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    static const Position npos;
    string getName() const;
    int getHP() const;
    int getExp() const;
};

class Character : public MovingObject
{
    friend class TestStudyInPink;

protected:
    string moving_rule;
    int hp;
    int exp;

public:
    Character(int index, const Position &init_pos, Map *map, const string &name);
    virtual ~Character();
    virtual Position getNextPosition();
    virtual Position getCurrentPosition() const;
    virtual void move();
    virtual string str() const = 0;
};
class Sherlock /* TODO */ : public Character
{
public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    string str() const;
    Position getNextPosition();
};

class Watson /* TODO */ : public Character
{
public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    string str() const;
    Position getNextPosition();
};

class Criminal /* TODO */ : public Character
{
private:
    Sherlock *sherlock;
    Watson *watson;
    int countSteps;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    string str() const;
    Position getNextPosition();
    void move();
    int getCountSteps() const
    {
        return countSteps;
    };
};

class Robot : public MovingObject
{
protected:
    RobotType robot_type;
    BaseItem *item;
    Criminal *criminal;
    Sherlock *sherlock;
    Watson *watson;
    int countRobots;

public:
    Robot(int index, const Position &init_pos, Map *map, RobotType robot_type);
    ~Robot();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition();
    void move();
};

class RobotC : public Robot
{
private:
public:
    RobotC(int index, const Position &init_pos, Map *map,
           RobotType robot_type, Criminal *criminal);
    Position getNextPosition();
    string str() const;
};

class RobotS : public Robot
{
private:
public:
    RobotS(int index, const Position &init_pos, Map *map,
           RobotType robot_type, Criminal *criminal, Sherlock *Sherlock);
    Position getNextPosition();
    string str() const;
};

class RobotW : public Robot
{
private:
public:
    RobotW(int index, const Position &init_pos, Map *map,
           RobotType robot_type, Criminal *criminal, Watson *watson);
    Position getNextPosition();
    string str() const;
};

class RobotSW : public Robot
{
private:
public:
    RobotSW(int index, const Position &init_pos, Map *map,
            RobotType robot_type, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    Position getNextPosition();
    Position getCurrentPosition() const;
    string str() const;
};

class BaseItem
{
protected:
    ItemType item_type;
    int value;

public:
    BaseItem(ItemType item_type, int value);
    virtual ~BaseItem();
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
};

class ArrayMovingObject
{
private:
    MovingObject *arr_mv_objs;
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
    friend class StudyPinkProgram;

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

class StudyPinkProgram
{
private:
    // Sample attributes
    Configuration *config;

    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

    Map *map;
    ArrayMovingObject *arr_mv_objs;

public:
    StudyPinkProgram(const string &config_file_path);

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

    ~StudyPinkProgram();
};