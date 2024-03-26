#include "game.h"

MapElement::MapElement(ElementType in_type) : type(in_type)
{
    // finish the constructor
    this->type = in_type;
}

MapElement::~MapElement()
{
}

ElementType MapElement::getType() const
{
    return type;
}

FakeWall::FakeWall(int r, int c) : MapElement(FAKE_WALL)
{
    req_exp = (r * 257 + c * 139 + 89) % 900 + 1;
}

int FakeWall::getReqExp() const
{
    return req_exp;
}

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
        map[array_fake_walls[i].getRow()][array_fake_walls[i].getCol()] = new FakeWall(array_fake_walls[i].getRow(), array_fake_walls[i].getCol());
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

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    if (pos.getRow() < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols)
    {
        return false;
    }
    if (map[pos.getRow()][pos.getCol()]->getType() == WALL)
    {
        return false;
    }
    if (map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL)
    {
        if (mv_obj->getName() == "Sherlock" || mv_obj->getName() == "Criminal")
        {
            return true;
        }
        else if (mv_obj->getName() == "Watson")
        {
            if (mv_obj->getExp() >= ((FakeWall *)map[pos.getRow()][pos.getCol()])->getReqExp())
            {
                return true;
            }
            return false;
        }
    }
    return true;
}

Position::Position(int r, int c)
{
    this->r = r;
    this->c = c;
}

Position::Position(const string &str_pos)
{
    /*Phương thức khởi tạo Constructor (public) với 1 tham số str_pos biểu diễn một vị trí
ở dạng chuỗi. Định dạng của str_pos là "(<r>,<c>)" với <r> và <c> lần lượt là giá
trị cho hàng và cột.*/
    sscanf(str_pos.c_str(), "(%d,%d)", &r, &c);
}
/*Phương thức isEqual có hai tham số truyền vào in_r và in_c biểu diễn cho một vị
trí. isEqual trả về giá trị true nếu vị trí truyền vào trùng với vị trí của đối tượng này.
Ngược lại, isEqual trả về false.*/
bool Position::isEqual(int in_r, int in_c) const
{
    if (r == in_r && c == in_c)
        return true;
    return false;
}

MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name = "")
{
    /*Phương thức gán giá trị của tham số cho thuộc
tính cùng tên. Riêng tham số name có giá trị mặc định là "".*/
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
}

MovingObject::~MovingObject()
{
    /*Phương thức hủy giải phóng bộ nhớ đã cấp phát cho thuộc tính map.*/
    delete map;
}

Position MovingObject::getNextPosition()
{
}

Position MovingObject::getCurrentPosition() const
{
    return pos;
}

const Position Position::npos(-1, -1);

string MovingObject::getName() const
{
    return name;
}

// isValid

Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, pos, map, "Sherlock")
{
    // finish the constructor
    this->moving_rule = moving_rule;
    this->hp = init_hp;
    this->exp = init_exp;
}

Position Character::getCurrentPosition() const
{
    return pos;
}

Position Character::getNextPosition()
{
    Position next_pos = pos;
    if (moving_rule.length() == 0)
    {
        return Position::npos;
    }
    int i = 0;
    while (true)
    {
        if (i == moving_rule.length())
        {
            i = 0;
        }
        if (moving_rule[i] == 'L')
        {
            next_pos.setCol(pos.getCol() - 1);
        }
        else if (moving_rule[i] == 'R')
        {
            next_pos.setCol(pos.getCol() + 1);
        }
        else if (moving_rule[i] == 'U')
        {
            next_pos.setRow(pos.getRow() - 1);
        }
        else if (moving_rule[i] == 'D')
        {
            next_pos.setRow(pos.getRow() + 1);
        }
        // check if the next position is valid with two parameter: pos and mv_obj
        if (map->isValid(next_pos, this))
        {
            return next_pos;
        }
        else
        {
            return pos;
        }
        i++;
    }
    return Position::npos;
}

void Character::move()
{
    // finish the method
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(-1, -1))
    {
        return;
    }
    pos = next_pos;
}

string Sherlock::str() const
{
    // Sherlock[index=<index>;pos=<pos>;moving_rule=<moving_rule>]
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

// do the same for class Watson
Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson")
{
    // finish the constructor
    this->moving_rule = moving_rule;
    this->hp = init_hp;
    this->exp = init_exp;
}

string Watson::str() const
{
    // Watson[index=<index>;pos=<pos>;moving_rule=<moving_rule>]
    return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

int MovingObject::getExp() const
{
    return exp;
}

string Criminal::str() const
{
    // Criminal[index=<index>;pos=<pos>]
    return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}

Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson) : Character(index, init_pos, map, "Criminal")
{
    this->sherlock = sherlock;
    this->watson = watson;
}

Position Criminal::getNextPosition()
{
    // finish the method
    Position next_pos = pos;
    int max_distance = -1;
    Position arr[4];
    // 4 elements of the arr is the position after moving U,L,D,R respectively, ex: (1,2)
    arr[0] = Position(pos.getRow() - 1, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() - 1);
    arr[2] = Position(pos.getRow() + 1, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() + 1);
    for (int i = 0; i < 4; i++)
    {
        if (map->isValid(arr[i], this))
        {
            int distance = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol()) + abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (distance > max_distance)
            {
                max_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == max_distance)
            {
                continue;
            }
        }
    }
    return next_pos;
}

ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    arr_mv_objs = new MovingObject *[capacity];
}

ArrayMovingObject::~ArrayMovingObject()
{
    for (int i = 0; i < capacity; i++)
    {
        delete arr_mv_objs[i];
    }
    delete[] arr_mv_objs;
}

bool ArrayMovingObject::isFull() const
{
    if (count == capacity)
    {
        return true;
    }
    return false;
}

bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    if (isFull())
    {
        return false;
    }
    arr_mv_objs[count] = mv_obj;
    count++;
    return true;
}

MovingObject *ArrayMovingObject::get(int index) const
{
    if (index < 0 || index >= count)
    {
        return nullptr;
    }
    return arr_mv_objs[index];
}

string ArrayMovingObject::str() const
{
    /*ArrayMovingObject [ count =3; capacity =10; Criminal [ index =0; pos =(8 ,9) ]; Sherlock [ index =1; pos (1 ,4) ; moving_rule = RUU ]; Watson [ index =2; pos=(2 ,1) ; moving_rule = LU ]]*/
    string res = "ArrayMovingObject [ count =" + to_string(count) + "; capacity =" + to_string(capacity) + "; ";
    for (int i = 0; i < count; i++)
    {
        res += arr_mv_objs[i]->str() + "[ " + "index = " + arr_mv_objs->index + "; pos =" + arr_mv_objs->pos.str() + "; moving_rule =" + arr_mv_objs->moving_rule + " ]";
    }
}
