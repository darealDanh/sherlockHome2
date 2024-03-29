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
bool Position::isEqual(const Position &pos) const
{
    if (r == pos.getRow() && c == pos.getCol())
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
MovingObject::MovingObject(const MovingObject &other)
{
    // Copy each member variable from `other` to `this`
    this->index = other.index;
    this->name = other.name;
    // Add other member variables as needed
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
    if (&next_pos == &Position::npos)
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

void Criminal::move()
{
    // finish the method
    Position next_pos = getNextPosition();
    if (&next_pos == &Position::npos)
    {
        return;
    }
    pos = next_pos;
    // after the criminal move 3 steps, create a new robot
    if (countSteps % 3 == 0)
    {
        /*Nếu là robot đầu tiên được tạo ra trên bản đồ, đó sẽ là loại robot RobotC. Nếu không,
ta xét khoảng cách từ Robot đến Sherlock và Watson:
• Nếu khoảng cách đến Sherlock gần hơn: Tạo ra loại robot RobotS
• Khoảng cách đến Watson gần hơn: Tạo ra loại robot RobotW
• Khoảng cách đến Sherlock và Watson là bằng nhau: Tạo ra loại robot RobotSW*/
    }
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
    arr_mv_objs[count] = *mv_obj;
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
        res += arr_mv_objs[i]->str() + "; ";
    }
    res += "]";
    return res;
}

void extractNum(const string &input, int &value)
{
    string temp = input;
    size_t pos = temp.find("=");
    temp = temp.substr(pos + 1);
    sscanf(temp.c_str(), "%d", &value);
}

void extractCoor(const string &input, string &value)
{
    string temp = input;
    size_t pos = temp.find("=");
    temp = temp.substr(pos + 1);
    value = temp;
}

Configuration::Configuration(const string &filepath)
{
    // finish the constructor
    int s = 0;
    int &value = s;
    int r, c;
    ifstream ifs(filepath);
    if (!ifs.is_open())
    {
        return;
    }
    string arr[12];
    for (int i = 0; i < 12; i++)
    {
        ifs >> arr[i];
    }
    for (int i = 0; i < 12; i++)
    {
        string s2;
        if (arr[i].find("MAP_NUM_ROWS"))
        {
            extractNum(arr[i], value);
            map_num_rows = value;
        }
        else if (arr[i].find("MAP_NUM_COLS"))
        {
            extractNum(arr[i], value);
            map_num_cols = value;
        }
        else if (arr[i].find("MAX_NUM_MOVING_OBJECTS"))
        {
            extractNum(arr[i], value);
            max_num_moving_objects = value;
        }
        else if (arr[i].find("NUM_WALLS"))
        {
            extractNum(arr[i], value);
            num_walls = value;
        }
        else if (arr[i].find("ARRAY_WALLS"))
        {
            string line;

            while (getline(ifs, line))
            {
                // remove ARRAY_WALLS first
                size_t pos = line.find("[");
                line = line.substr(pos);
                line = line.substr(1, line.size() - 2); // Remove brackets

                stringstream ss(line);
                string pair;
                int i = 0;
                while (getline(ss, pair, ';'))
                {
                    // Process the pair string immediately
                    arr_walls[i] = Position(pair);
                }
            }
        }
        else if (arr[i].find("NUM_FAKE_WALLS"))
        {
            extractNum(arr[i], value);
            num_fake_walls = value;
        }
        else if (arr[i].find("ARRAY_FAKE_WALLS"))
        {
            string line;

            while (getline(ifs, line))
            {
                // remove ARRAY_WALLS first
                size_t pos = line.find("[");
                line = line.substr(pos);
                line = line.substr(1, line.size() - 2); // Remove brackets

                stringstream ss(line);
                string pair;
                int i = 0;
                while (getline(ss, pair, ';'))
                {
                    // Process the pair string immediately
                    arr_fake_walls[i] = Position(pair);
                }
            }
        }
        else if (arr[i].find("SHERLOCK_INIT_HP"))
        {
            extractNum(arr[i], value);
            sherlock_init_hp = value;
        }
        else if (arr[i].find("SHERLOCK_INIT_EXP"))
        {
            extractNum(arr[i], value);
            sherlock_init_exp = value;
        }
        else if (arr[i].find("WATSON_INIT_HP"))
        {
            extractNum(arr[i], value);
            watson_init_hp = value;
        }
        else if (arr[i].find("WATSON_INIT_EXP"))
        {
            extractNum(arr[i], value);
            watson_init_exp = value;
        }
        else if (arr[i].find("SHERLOCK_INIT_POS"))
        {
            extractCoor(arr[i], s2);
            sherlock_init_pos = new Position(s2);
        }
        else if (arr[i].find("WATSON_INIT_POS"))
        {
            extractCoor(arr[i], s2);
            watson_init_pos = new Position(s2);
        }
        else if (arr[i].find("CRIMINAL_INIT_POS"))
        {
            extractCoor(arr[i], s2);
            criminal_init_pos = new Position(s2);
        }
        else if (arr[i].find("SHERLOCK_MOVING_RULE"))
        {
            extractCoor(arr[i], s2);
            sherlock_moving_rule = s2;
        }
        else if (arr[i].find("WATSON_MOVING_RULE"))
        {
            extractCoor(arr[i], s2);
            watson_moving_rule = s2;
        }
        else if (arr[i].find("NUM_STEPS"))
        {
            extractNum(arr[i], value);
            num_steps = value;
        }
    }
    ifs.close();
}

Configuration::~Configuration()
{
    delete[] arr_walls;
    delete[] arr_fake_walls;
    delete sherlock_init_pos;
    delete watson_init_pos;
    delete criminal_init_pos;
}

string Configuration::str() const
{
    /*Configuration[
    MAP_NUM_ROWS=10
    MAP_NUM_COLS=10
    MAX_NUM_MOVING_OBJECTS=10
    NUM_WALLS=3
    ARRAY_WALLS=[(1,2);(2,3);(3,4)]
    NUM_FAKE_WALLS=1
    ARRAY_FAKE_WALLS=[(4,5)]
    SHERLOCK_MOVING_RULE=RUU
    SHERLOCK_INIT_POS=(1,3)
    SHERLOCK_INIT_HP=250
    SHERLOCK_INIT_EXP=500
    WATSON_MOVING_RULE=LU
    WATSON_INIT_POS=(2,1)
    WATSON_INIT_HP=300
    WATSON_INIT_EXP=350
    CRIMINAL_INIT_POS=(7,9)
    NUM_STEPS=100]*/
    string res = "Configuration[\n";
    res += "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
    res += "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
    res += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
    res += "NUM_WALLS=" + to_string(num_walls) + "\n";
    res += "ARRAY_WALLS=[";
    for (int i = 0; i < num_walls; i++)
    {
        res += arr_walls[i].str() + ";";
    }
    res += "]\n";
    res += "NUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\n";
    res += "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < num_fake_walls; i++)
    {
        res += arr_fake_walls[i].str() + ";";
    }
    res += "]\n";
    res += "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\n";
    res += "SHERLOCK_INIT_POS=" + sherlock_init_pos->str() + "\n";
    res += "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\n";
    res += "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\n";
    res += "WATSON_MOVING_RULE=" + watson_moving_rule + "\n";
    res += "WATSON_INIT_POS=" + watson_init_pos->str() + "\n";
    res += "WATSON_INIT_HP=" + to_string(watson_init_hp) + "\n";
    res += "WATSON_INIT_EXP=" + to_string(watson_init_exp) + "\n";
    res += "CRIMINAL_INIT_POS=" + criminal_init_pos->str() + "\n";
    res += "NUM_STEPS=" + to_string(num_steps) + "\n";
    res += "]";
    return res;
}

Robot::Robot(int index, const Position &init_pos, Map *map, RobotType robot_type) : MovingObject(index, init_pos, map, name)
{
    this->robot_type = robot_type;
}

Position Robot::getCurrentPosition()
{
    return pos;
}

void Robot::move()
{
    Position next_pos = getNextPosition();
    if (&next_pos == &Position::npos)
    {
        return;
    }
    pos = next_pos;
}

RobotC::RobotC(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal) : Robot(index, init_pos, map, C)
{
    this->criminal = criminal;
};

Position RobotC::getNextPosition()
{
    // go to next_pos with criminal
    Position next_pos = criminal->getCurrentPosition();
    return next_pos;
};

RobotS::RobotS(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal, Sherlock *Sherlock) : Robot(index, init_pos, map, S)
{
    this->sherlock = Sherlock;
    this->criminal = criminal;
};

Position RobotS::getNextPosition()
{
    /*Di chuyển đến vị trí tiếp theo cách ban đầu 1 đơn vị và gần nhất với vị trí
tiếp theo của Sherlock. Lưu ý khi nói vị trí cách vị trí ban đầu 1 đơn vị thì khoảng
cách được nói đến là khoảng cách Manhattan. Nếu có nhiều vị trí gần nhất thì thứ
tự lựa chọn theo chiều quay của kim đồng hồ , bắt đầu từ hướng đi lên, và
lựa chọn vị trí đầu tiên*/
    Position next_pos = pos;
    int min_distance = 1000000;
    Position arr[4];
    arr[0] = Position(pos.getRow() - 1, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() + 1);
    arr[2] = Position(pos.getRow() + 1, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() - 1);
    for (int i = 0; i < 4; i++)
    {
        if (map->isValid(arr[i], this))
        {
            int distance = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol());
            if (distance < min_distance)
            {
                min_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == min_distance)
            {
                continue;
            }
        }
    }
    return next_pos;
};

RobotW::RobotW(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal, Watson *watson) : Robot(index, init_pos, map, W)
{
    this->watson = watson;
    this->criminal = criminal;
};

RobotSW::RobotSW(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal, Sherlock *Sherlock, Watson *watson) : Robot(index, init_pos, map, SW)
{
    this->sherlock = Sherlock;
    this->watson = watson;
    this->criminal = criminal;
};

int main()
{
}