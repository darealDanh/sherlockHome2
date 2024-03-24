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
    FakeWall(int in_req_exp, int r, int c);
    int getReqExp() const;
};

class Map
{
private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls) : num_rows(num_rows), num_cols(num_cols);
    ~Map();
    // bool isValid ( const Position & pos , MovingObject * mv_obj ) const ;
};

class Position
{
private:
    int r, c;

public:
    static const Position npos;

    Position(int r = 0, int c = 0);

    Position(const string &str_pos);

    int getRow() const { return r; };
    int getCol() const { return c; };
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;
};

class MovingObject
{
protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    virtual Position getCurrentPosition() const = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    static const Position npos;
};

class Sherlock /* TODO */ : public MovingObject
{
private:
    string moving_rule;
    int hp;
    int exp;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    // getNextPosition
    // move
    // str
    // ...
    Position getNextPosition();
    Position getCurrentPosition() const;
    void move();
    string str() const;
};

class Watson /* TODO */ : public MovingObject
{
private:
    string moving_rule;
    int hp;
    int exp;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    // getNextPosition
    // move
    // str
    // ...
    Position getNextPosition();
    Position getCurrentPosition() const;
    void move();
    string str() const;
};

class Criminal /* TODO */ : public MovingObject
{
private:
    // TOD

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    // getNextPosition
    // move
    // str
    // ...
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
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
        {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
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