#include "game.h"

MapElement::MapElement(ElementType in_type) : type(in_type)
{
    // finish the constructor
    this->type = in_type;
}

FakeWall::FakeWall(int in_req_exp, int r, int c) : MapElement(FAKE_WALL)
{
    in_req_exp = (r * 257 + c * 139 + 89) % 900 + 1;
}

int FakeWall::getReqExp() const
{
    return req_exp;
}

// define constructor of class map that creates a 2d map of map elements, if the elements is in array_wall then it is a wall, if it is in array_fake_wall then it is a fake wall, else it is a path
Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls) : num_rows(num_rows), num_cols(num_cols)
{

    map = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; i++)
    {
        map[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; j++)
        {
            map[i][j] = new Path();
        }
    }

    for (int i = 0; i < num_walls; i++)
    {
        map[array_walls[i].getRow()][array_walls[i].getCol()] = new Wall();
    }

    for (int i = 0; i < num_fake_walls; i++)
    {
        map[array_fake_walls[i].getRow()][array_fake_walls[i].getCol()] = new FakeWall(0, array_fake_walls[i].getRow(), array_fake_walls[i].getCol());
    }
}

Map::~Map()
{
    for (int i = 0; i < num_rows; i++)
    {
        delete[] map[i];
    }
    delete[] map;
}
