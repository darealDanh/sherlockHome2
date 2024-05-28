#include "study_in_pink21.h"

MapElement::MapElement(ElementType in_type)
{
    this->type = in_type;
}

MapElement::~MapElement()
{
}

ElementType MapElement::getType() const
{
    return type;
}

Path::Path() : MapElement(PATH)
{
    type = PATH;
};

Wall::Wall() : MapElement(WALL)
{
    type = WALL;
}

FakeWall::FakeWall(int in_req_exp) : MapElement(FAKE_WALL)
{
    req_exp = in_req_exp;
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
        // calculate in req_exp (r ∗ 257 + c ∗ 139 + 89)%900 + 1
        int req_exp = (array_fake_walls[i].getRow() * 257 + array_fake_walls[i].getCol() * 139 + 89) % 900 + 1;
        map[array_fake_walls[i].getRow()][array_fake_walls[i].getCol()] = new FakeWall(req_exp);
    }
}

MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
{
    /*Phương thức gán giá trị của tham số cho thuộc
tính cùng tên. Riêng tham số name có giá trị mặc định là "".*/
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
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

void MovingObject::setHP(int hp)
{
    this->hp = hp;
}

void MovingObject::setEXP(int exp)
{
    this->exp = exp;
}

int MovingObject::getHP() const
{
    return hp;
}

int MovingObject::getEXP() const
{
    return exp;
}

Map::~Map()
{
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            delete map[i][j];
        }
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
        if (mv_obj->getName() == "Sherlock" || mv_obj->getName() == "Criminal" || mv_obj->getName() == "RobotC" || mv_obj->getName() == "RobotS" || mv_obj->getName() == "RobotW" || mv_obj->getName() == "RobotSW")
        {
            return true;
        }
        else if (mv_obj->getName() == "Watson")
        {
            if (mv_obj->getEXP() >= ((FakeWall *)map[pos.getRow()][pos.getCol()])->getReqExp())
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
    sscanf(str_pos.c_str(), "(%d,%d)", &r, &c);
}

bool Position::isEqual(const Position &pos) const
{
    if (r == pos.getRow() && c == pos.getCol())
        return true;
    return false;
}
bool Position::isEqual(int r, int c) const
{
    if (r == this->r && c == this->c)
        return true;
    return false;
}

Character::Character(int index, const Position &init_pos, Map *map, const string &name) : MovingObject(index, init_pos, map, name)
{
    this->steps = 0;
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
        next_pos = Position::npos;
    }
    if (steps == moving_rule.length())
    {
        steps = 0;
    }
    int temp = 0;
    if (moving_rule[steps] == 'L')
    {
        next_pos.setCol(pos.getCol() - 1);
        temp += 1;
        steps++;
    }
    else if (moving_rule[steps] == 'R')
    {
        next_pos.setCol(pos.getCol() + 1);
        temp += 1;
        steps++;
    }
    else if (moving_rule[steps] == 'U')
    {
        next_pos.setRow(pos.getRow() - 1);
        temp += 1;
        steps++;
    }
    else if (moving_rule[steps] == 'D')
    {
        next_pos.setRow(pos.getRow() + 1);
        temp += 1;
        steps++;
    }
    // check if the next position is valid with two parameter: pos and mv_obj
    if (map->isValid(next_pos, this))
    {
        return next_pos;
    }
    else
    {
        next_pos = pos;
    }
    if (temp == 0)
    {
        next_pos = Position::npos;
    }
    return next_pos;
}

BaseBag *Character::getBag() const
{
    return bag;
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

Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock")
{
    // finish the constructor
    this->moving_rule = moving_rule;
    this->hp = init_hp;
    this->exp = init_exp;
    bag = new SherlockBag(this);
}

string Sherlock::str() const
{
    // Sherlock[index=<index>;pos=<pos>;moving_rule=<moving_rule>]
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

void Sherlock::move()
{
    Position next_pos = getNextPosition();
    if (&next_pos == &Position::npos)
    {
        return;
    }
    pos = next_pos;
}

bool Sherlock::meetRobotC(Robot *robot)
{
    if (pos.isEqual(robot->getCurrentPosition()))
    {
        if (bag->get(PASSING_CARD) != nullptr)
        {
            BaseItem *item = this->bag->get(PASSING_CARD);
            item->use(this, robot);
        }
        else
        {
            if (this->getEXP() > 500)
            {
                pos = robot->getCriminalPos();
            }
            else
            {
                BaseItem *item = robot->getItem();
                this->bag->insert(item);
            }
        }
        ItemType item = MAGIC_BOOK;
        BaseItem *magicbook = this->bag->get(item);
        while (magicbook != nullptr)
        {
            magicbook->use(this, robot);
            magicbook = this->bag->get(item);
        }
        item = FIRST_AID;
        BaseItem *firstaid = this->bag->get(item);
        while (firstaid != nullptr)
        {
            firstaid->use(this, robot);
            firstaid = this->bag->get(item);
        }
        item = ENERGY_DRINK;
        BaseItem *energydrink = this->bag->get(item);
        while (energydrink != nullptr)
        {
            energydrink->use(this, robot);
            energydrink = this->bag->get(item);
        }
        return true;
    }
    return false;
}

bool Sherlock::meetRobot(Robot *robot)
{
    if (pos.isEqual(robot->getCurrentPosition()))
    {
        if (robot->getRobotType() == 1)
        {
            if (this->getEXP() > 400)
            {
                BaseItem *item = robot->getItem();
                this->bag->insert(item);
            }
            else
            {
                this->setEXP(this->getEXP() * 0.9);
            }
        }
        if (robot->getRobotType() == 2)
        {
            BaseItem *item = robot->getItem();
            this->bag->insert(item);
        }
        if (robot->getRobotType() == 3)
        {
            if (this->getEXP() > 300 && this->getHP() > 335)
            {
                BaseItem *item = robot->getItem();
                this->bag->insert(item);
            }
            else
            {
                this->setEXP(this->getEXP() * 0.85);
                this->setHP(this->getHP() * 0.85);
            }
        }
        ItemType item = MAGIC_BOOK;
        BaseItem *magicbook = this->bag->get(item);
        while (magicbook != nullptr)
        {
            magicbook->use(this, robot);
            magicbook = this->bag->get(item);
        }
        item = FIRST_AID;
        BaseItem *firstaid = this->bag->get(item);
        while (firstaid != nullptr)
        {
            firstaid->use(this, robot);
            firstaid = this->bag->get(item);
        }
        item = ENERGY_DRINK;
        BaseItem *energydrink = this->bag->get(item);
        while (energydrink != nullptr)
        {
            energydrink->use(this, robot);
            energydrink = this->bag->get(item);
        }
        return true;
    }
    return false;
}
bool Sherlock::meetWatson(Watson *watson)
{
    // check if there are any passing card in sherlockBag;
    if (pos.isEqual(watson->getCurrentPosition()))
    {
        ItemType item = PASSING_CARD;
        BaseItem *pcard = this->bag->get(item);
        if (pcard == nullptr || watson->getBag() == nullptr)
        {
            return false;
        }
        while (pcard != nullptr)
        {
            watson->getBag()->insert(pcard);
            pcard = this->bag->get(item);
            if (pcard == nullptr || watson->getBag() == nullptr)
            {
                break;
            }
        }
        return true;
    }
    return false;
}

Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson")
{
    this->moving_rule = moving_rule;
    this->hp = init_hp;
    this->exp = init_exp;
    bag = new WatsonBag(this);
}

string Watson::str() const
{
    // Watson[index=<index>;pos=<pos>;moving_rule=<moving_rule>]
    return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

bool Watson::meetSherlock(Sherlock *sherlock)
{
    // check if there are any exemcemption card in watsonBag;
    if (pos.isEqual(sherlock->getCurrentPosition()))
    {
        ItemType item = EXCEMPTION_CARD;
        BaseItem *ecard = this->bag->get(item);
        while (ecard != nullptr)
        {
            sherlock->getBag()->insert(ecard);
            ecard = this->bag->get(item);
        }
        return true;
    }
    return false;
}

bool Watson::meetRobotC(Robot *robot)
{
    if (pos.isEqual(robot->getCurrentPosition()))
    {
        BaseItem *itemGet = robot->getItem();
        this->bag->insert(itemGet);
        robot = nullptr;
        ItemType item = MAGIC_BOOK;
        BaseItem *magicbook = this->bag->get(item);
        while (magicbook != nullptr)
        {
            magicbook->use(this, robot);
            magicbook = this->bag->get(item);
        }
        item = FIRST_AID;
        BaseItem *firstaid = this->bag->get(item);
        while (firstaid != nullptr)
        {
            firstaid->use(this, robot);
            firstaid = this->bag->get(item);
        }
        item = ENERGY_DRINK;
        BaseItem *energydrink = this->bag->get(item);
        while (energydrink != nullptr)
        {
            energydrink->use(this, robot);
            energydrink = this->bag->get(item);
        }
        return true;
    }
    return false;
}

bool Watson::meetRobot(Robot *robot)
{
    if (pos.isEqual(robot->getCurrentPosition()))
    {
        if (this->bag->get(EXCEMPTION_CARD) != nullptr)
        {
            BaseItem *item = this->bag->get(EXCEMPTION_CARD);
            item->use(this, robot);
            robot = nullptr;
            return true;
        }
        else
        {
            if (robot->getRobotType() == 2)
            {
                if (this->getHP() > 350)
                {
                    BaseItem *item = robot->getItem();
                    this->bag->insert(item);
                    robot = nullptr;
                }
                else
                {
                    this->setHP(this->getHP() * 0.95);
                }
            }
            if (robot->getRobotType() == 3)
            {
                if (this->getEXP() > 600 && this->getHP() > 165)
                {
                    BaseItem *item = robot->getItem();
                    this->bag->insert(item);
                    robot = nullptr;
                }
                else
                {
                    this->setEXP(this->getEXP() * 0.85);
                    this->setHP(this->getHP() * 0.85);
                }
            }
        }
        ItemType item = MAGIC_BOOK;
        BaseItem *magicbook = this->bag->get(item);
        while (magicbook != nullptr)
        {
            magicbook->use(this, robot);
            magicbook = this->bag->get(item);
        }
        item = FIRST_AID;
        BaseItem *firstaid = this->bag->get(item);
        while (firstaid != nullptr)
        {
            firstaid->use(this, robot);
            firstaid = this->bag->get(item);
        }
        item = ENERGY_DRINK;
        BaseItem *energydrink = this->bag->get(item);
        while (energydrink != nullptr)
        {
            energydrink->use(this, robot);
            energydrink = this->bag->get(item);
        }
        return true;
    }
    return false;
}

void Watson::move()
{
    Position next_pos = getNextPosition();
    if (&next_pos == &Position::npos)
    {
        return;
    }
    pos = next_pos;
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
    this->prev_pos = Position::npos;
}

Position Criminal::getNextPosition()
{
    // finish the method
    Position next_pos = pos;
    prev_pos = pos;
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
    if (&next_pos == &pos)
    {
        next_pos = Position::npos;
    }
    return next_pos;
}

void Criminal::move()
{
    Position next_pos = getNextPosition();
    if (&next_pos == &Position::npos)
    {
        return;
    }
    pos = next_pos;
}

Robot *Criminal::getRobot() const
{
    return robot;
}

Position Criminal::getPrevPos() const
{
    return prev_pos;
}

ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    this->count = 0;
    arr_mv_objs = new MovingObject *[capacity];
    // set every element of the array to nullptr
    for (int i = 0; i < capacity; i++)
    {
        arr_mv_objs[i] = nullptr;
    }
}

ArrayMovingObject::~ArrayMovingObject()
{
    if (arr_mv_objs != nullptr)
    {
        delete[] arr_mv_objs;
        arr_mv_objs = nullptr;
    }
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

int ArrayMovingObject::size() const
{
    return count;
}

string ArrayMovingObject::str() const
{
    /*ArrayMovingObject [ count =3; capacity =10; Criminal [ index =0; pos =(8 ,9) ]; Sherlock [ index =1; pos (1 ,4) ; moving_rule = RUU ]; Watson [ index =2; pos=(2 ,1) ; moving_rule = LU ]]*/
    string res = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity) + ";";
    for (int i = 0; i < count; i++)
    {
        res += get(i)->str();
        if (i != count - 1)
        {
            res += ";";
        }
    }
    res += "]";
    return res;
}

Configuration::Configuration(const string &filepath)
{
    ifstream ifs(filepath);
    if (!ifs.is_open())
    {
        exit(1);
    }
    string s;
    // MAP_NUM_ROWS=10
    for (int i = 0; i < 15 && !ifs.eof(); i++)
    {
        getline(ifs, s);
        string temp;
        if (s.find("MAP_NUM_ROWS") == 0)
        {
            map_num_rows = stoi(s.substr(s.find('=') + 1));
        }
        else if (s.find("MAP_NUM_COLS") == 0)
        {
            map_num_cols = stoi(s.substr(s.find('=') + 1));
        }
        else if (s.find("MAX_NUM_MOVING_OBJECTS") == 0)
        {
            max_num_moving_objects = stoi(s.substr(s.find('=') + 1));
        }
        else if (s.find("NUM_WALLS") == 0)
        {
            num_walls = stoi(s.substr(s.find('=') + 1));
        }
        else if (s.find("NUM_FAKE_WALLS") == 0)
        {
            num_fake_walls = stoi(s.substr(s.find('=') + 1));
        }
        else if (s.find("ARRAY_WALLS") == 0)
        {
            int numWall = 0;
            for (int i = 0; i < s.length(); i++)
            {
                if (s[i] == ';')
                {
                    numWall++;
                }
            }
            num_walls = ++numWall;

            if (num_walls > 0)
            {
                arr_walls = new Position[num_walls];
                string start = s.substr(s.find('[') + 1, s.find(']') - s.find('[') - 1);
                for (int i = 0; i < num_walls; i++)
                {
                    string pair = start.substr(start.find('('), start.find(')') - start.find('(') + 1);
                    arr_walls[i] = Position(pair);
                    start = start.substr(start.find(')') + 1);
                }
            }
        }
        else if (s.find("ARRAY_FAKE_WALLS") == 0)
        {
            int numFakeWall = 0;
            for (int i = 0; i < s.length(); i++)
            {
                if (s[i] == ';')
                {
                    numFakeWall++;
                }
            }
            num_fake_walls = ++numFakeWall;

            if (num_fake_walls > 0)
            {
                arr_fake_walls = new Position[num_fake_walls];
                string start = s.substr(s.find('[') + 1, s.find(']') - s.find('[') - 1);
                for (int i = 0; i < num_fake_walls; i++)
                {
                    string pair = start.substr(start.find('('), start.find(')') - start.find('(') + 1);
                    arr_fake_walls[i] = Position(pair);
                    start = start.substr(start.find(')') + 1);
                }
            }
        }
        else if (s.find("SHERLOCK_MOVING_RULE") == 0)
        {
            sherlock_moving_rule = s.substr(s.find('=') + 1);
        }
        else if (s.find("SHERLOCK_INIT_POS") == 0)
        {
            sherlock_init_pos = Position(s.substr(s.find('=') + 1));
        }
        else if (s.find("SHERLOCK_INIT_HP") == 0)
        {
            sherlock_init_hp = stoi(s.substr(s.find('=') + 1));
        }
        else if (s.find("SHERLOCK_INIT_EXP") == 0)
        {
            sherlock_init_exp = stoi(s.substr(s.find('=') + 1));
        }
        else if (s.find("WATSON_MOVING_RULE") == 0)
        {
            watson_moving_rule = s.substr(s.find('=') + 1);
        }
        else if (s.find("WATSON_INIT_POS") == 0)
        {
            watson_init_pos = Position(s.substr(s.find('=') + 1));
        }
        else if (s.find("WATSON_INIT_HP") == 0)
        {
            watson_init_hp = stoi(s.substr(s.find('=') + 1));
        }
        else if (s.find("WATSON_INIT_EXP") == 0)
        {
            watson_init_exp = stoi(s.substr(s.find('=') + 1));
        }
        else if (s.find("CRIMINAL_INIT_POS") == 0)
        {
            criminal_init_pos = Position(s.substr(s.find('=') + 1));
        }
        else if (s.find("NUM_STEPS") == 0)
        {
            num_steps = stoi(s.substr(s.find('=') + 1));
        }
    }
}
Configuration::~Configuration()
{
    delete[] arr_walls;
    delete[] arr_fake_walls;
    arr_walls = nullptr;
    arr_fake_walls = nullptr;
}

string Configuration::str() const
{
    string res = "Configuration[\n";
    res += "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
    res += "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
    res += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
    res += "NUM_WALLS=" + to_string(num_walls) + "\n";
    res += "ARRAY_WALLS=[";
    for (int i = 0; i < num_walls; i++)
    {
        res += arr_walls[i].str();
        if (i != num_walls - 1)
        {
            res += ";";
        }
    }
    res += "]\n";
    res += "NUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\n";
    res += "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < num_fake_walls; i++)
    {
        res += arr_fake_walls[i].str();
        if (i != num_fake_walls - 1)
        {
            res += ";";
        }
    }
    res += "]\n";
    res += "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\n";
    res += "SHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\n";
    res += "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\n";
    res += "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\n";
    res += "WATSON_MOVING_RULE=" + watson_moving_rule + "\n";
    res += "WATSON_INIT_POS=" + watson_init_pos.str() + "\n";
    res += "WATSON_INIT_HP=" + to_string(watson_init_hp) + "\n";
    res += "WATSON_INIT_EXP=" + to_string(watson_init_exp) + "\n";
    res += "CRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\n";
    res += "NUM_STEPS=" + to_string(num_steps) + "\n";
    res += "]";
    return res;
}

Robot::Robot(int index, const Position &init_pos, Map *map, Criminal *criminal, RobotType robot_type) : MovingObject(index, init_pos, map, "Robot")

{
    this->robot_type = robot_type;
    this->criminal = criminal;
    /*Gọi vị trí tạo ra robot có toạ độ là (i,j) với i là chỉ số hàng, j là chỉ số cột.
Với p = i ∗ j. Gọi s số chủ đạo của p . Ta định nghĩa số chủ đạo của một số là giá trị tổng
các chữ số, cho đến khi giá trị tổng đó là số có 1 chữ số*/
    int p = init_pos.getRow() * init_pos.getCol();
    int t = (init_pos.getRow() * 11 + init_pos.getCol()) % 4;
    int s = 0;
    while (p > 0)
    {
        s += p % 10;
        p /= 10;
    }
    while (s >= 10)
    {
        p = s;
        s = 0;
        while (p > 0)
        {
            s += p % 10;
            p /= 10;
        }
    }
    if (s >= 0 && s <= 1)
    {
        *item = MagicBook();
    }
    else if (s >= 2 && s <= 3)
    {
        *item = EnergyDrink();
    }
    else if (s >= 4 && s <= 5)
    {
        *item = FirstAid();
    }
    else if (s >= 6 && s <= 7)
    {
        *item = ExcemptionCard();
    }
    else if (s >= 8 && s <= 9)
    {
        switch (t)
        {
        case 0:
            *item = PassingCard("RobotS");
            break;
        case 1:
            *item = PassingCard("RobotC");
            break;
        case 2:
            *item = PassingCard("RobotSW");
            break;
        case 3:
            *item = PassingCard("all");
            break;

        default:
            break;
        }
    }
};

Position Robot::getCurrentPosition() const
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

BaseItem *Robot::getItem()
{
    return item;
}

int Robot::countRobots = 0;

Robot *Robot::create(int index, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
{
    Robot *robot;
    Position pos = criminal->getCurrentPosition();
    if (Robot::countRobots == 0)
    {
        *robot = RobotC(0, pos, map, criminal);
    }
    else
    {
        int distanceS = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
        int distanceW = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
        if (distanceS < distanceW)
        {
            *robot = RobotS(0, pos, map, criminal, sherlock);
        }
        else if (distanceS > distanceW)
        {
            *robot = RobotW(0, pos, map, criminal, watson);
        }
        else
        {
            *robot = RobotSW(0, pos, map, criminal, sherlock, watson);
        }
    }
    return robot;
}
Position Robot::getCriminalPos() const
{
    return criminal->getCurrentPosition();
}

RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal) : Robot(index, init_pos, map, criminal, C)
{
    this->name = "RobotC";
};

Position RobotC::getNextPosition()
{
    // go to next_pos with criminal
    Position next_pos = criminal->getPrevPos();
    if (&next_pos == &pos)
    {
        next_pos = Position::npos;
    }
    return next_pos;
};

string RobotC::str() const
{
    // Robot[pos=<pos>;type=<robot_type>;dist=<empty>]
    return "Robot[pos=" + pos.str() + ";type=C" + ";dist=]";
}

int RobotC::getDistance(Character *character) const
{
    /*Riêng robot loại C cung cấp 2 phương thức tính toán khoảng cách đến
Sherlock hoặc đến Watson dựa trên 1 tham số đầu vào là con trỏ Sherlock* sherlock
hay Watson* watson.*/
    return abs(pos.getRow() - character->getCurrentPosition().getRow()) + abs(pos.getCol() - character->getCurrentPosition().getCol());
};

RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *Sherlock) : Robot(index, init_pos, map, criminal, S)
{
    this->sherlock = Sherlock;
    this->name = "RobotS";
};

int RobotS::getDistance() const
{
    return abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
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
    if (&next_pos == &pos)
    {
        next_pos = Position::npos;
    }
    return next_pos;
};

string RobotS::str() const
{
    // Robot[pos=<pos>;type=<robot_type>;dist=<dist>]
    return "Robot[pos=" + pos.str() + ";type=S" + ";dist=" + to_string(getDistance()) + "]";
}

RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson) : Robot(index, init_pos, map, criminal, W)
{
    this->watson = watson;
    this->name = "RobotW";
};

Position RobotW::getNextPosition()
{
    /*Di chuyển đến vị trí tiếp theo cách ban đầu 1 đơn vị và gần nhất với vị
trí tiếp theo của Watson. Nếu có nhiều vị trí phù hợp thì thứ tự lựa chọn như nếu
trong RobotS*/
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
            int distance = abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
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
    if (&next_pos == &pos)
    {
        next_pos = Position::npos;
    }
    return next_pos;
}

int RobotW::getDistance() const
{
    return abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
};

string RobotW::str() const
{
    // Robot[pos=<pos>;type=<robot_type>;dist=<dist>]
    return "Robot[pos=" + pos.str() + ";type=W" + ";dist=" + to_string(getDistance()) + "]";
}

RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *Sherlock, Watson *watson) : Robot(index, init_pos, map, criminal, SW)
{
    this->sherlock = Sherlock;
    this->watson = watson;
    this->name = "RobotSW";
};

Position RobotSW::getNextPosition()
{
    /*Di chuyển đến vị trí tiếp theo cách ban đầu 2 đơn vị và có tổng khoảng
cách đến cả Sherlock và Watson là gần nhất. Nếu có nhiều vị trí phù hợp thì thứ tự
lựa chọn như nếu trong RobotS.*/
    Position next_pos = pos;
    int min_distance = 1000000;
    Position arr[4];
    arr[0] = Position(pos.getRow() - 2, pos.getCol());
    arr[1] = Position(pos.getRow(), pos.getCol() + 2);
    arr[2] = Position(pos.getRow() + 2, pos.getCol());
    arr[3] = Position(pos.getRow(), pos.getCol() - 2);
    for (int i = 0; i < 4; i++)
    {
        if (map->isValid(arr[i], this))
        {
            int distance = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol()) + abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
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
    if (&next_pos == &pos)
    {
        next_pos = Position::npos;
    }
    return next_pos;
}

int RobotSW::getDistance() const
{
    return abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol()) + abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
};

string RobotSW::str() const
{
    // Robot[pos=<pos>;type=<robot_type>;dist=<dist>]
    return "Robot[pos=" + pos.str() + ";type=SW" + ";dist=" + to_string(getDistance()) + "]";
}

bool BaseItem::canUse(Character *obj, Robot *robot)
{
    // get the object inside the robot and check if the obj can use the item
    if ((robot->getItem())->canUse(obj, robot))
    {
        return true;
    };
    return false;
}

ItemType MagicBook::getItemType() const
{
    return MAGIC_BOOK;
}

ItemType EnergyDrink::getItemType() const
{
    return ENERGY_DRINK;
}

ItemType FirstAid::getItemType() const
{
    return FIRST_AID;
}

ItemType ExcemptionCard::getItemType() const
{
    return EXCEMPTION_CARD;
}

ItemType PassingCard::getItemType() const
{
    return PASSING_CARD;
}

bool MagicBook::canUse(Character *obj, Robot *robot)
{
    if (obj->getEXP() >= 350)
    {
        return true;
    }
    return false;
}

void MagicBook::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        obj->setEXP(obj->getEXP() * 1.25);
    }
}

bool EnergyDrink::canUse(Character *obj, Robot *robot)
{
    if (obj->getHP() <= 100)
    {
        return true;
    }
    return false;
}

void EnergyDrink::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        obj->setHP(obj->getHP() * 1.2);
    }
}

bool FirstAid::canUse(Character *obj, Robot *robot)
{
    if (obj->getHP() <= 100 || obj->getEXP() <= 350)
    {
        return true;
    }
    return false;
}

void FirstAid::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        obj->setHP(obj->getHP() * 1.5);
    }
}

bool ExcemptionCard::canUse(Character *obj, Robot *robot)
{
    if (obj->getName() == "Sherlock" && obj->getHP() % 2 != 0)
    {
        return true;
    }
    return false;
}

void ExcemptionCard::use(Character *obj, Robot *robot)
{
    obj->setEXP(obj->getEXP());
    obj->setHP(obj->getHP());
}

PassingCard::PassingCard(const string &challenges)
{
    this->challenge = challenges;
}

bool PassingCard::canUse(Character *obj, Robot *robot)
{
    if (obj->getName() == "Watson" && obj->getEXP() % 2 != 0)
    {
        return true;
    }
    return false;
}

void PassingCard::use(Character *obj, Robot *robot)
{
    if (challenge == robot->getName() || challenge == "all")
    {
        BaseItem *item = robot->getItem();
        obj->getBag()->insert(item);
    }
    else
    {
        obj->setEXP(obj->getEXP() - 50);
        BaseItem *item = robot->getItem();
        obj->getBag()->insert(item);
    }
}

BaseBag::BaseBag(int capacity)
{
    this->head = nullptr;
    this->countItem = 0;
    this->capacity = capacity;
}

BaseBag::~BaseBag()
{
    while (head != nullptr)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
}

bool BaseBag::insert(BaseItem *item)
{
    // add to the start of the list
    if (character->getName() == "Sherlock")
    {
        if (countItem <= 13)
        {
            Node *newNode = new Node(item, head);
            head = newNode;
            countItem++;
            return true;
        }
    }
    if (character->getName() == "Watson")
    {
        if (countItem <= 15)
        {
            Node *newNode = new Node(item, head);
            head = newNode;
            countItem++;
            return true;
        }
    }
    return false;
};

BaseItem *BaseBag::get()
{
    Node *current = head;
    while (current != nullptr)
    {
        if (current->item->canUse(character, nullptr))
        {
            // This item can be used, so remove it from the list and return it
            if (current == head)
            {
                // The item is at the head of the list
                head = current->next;
            }
            else
            {
                // The item is somewhere in the middle or at the end of the list
                Node *previous = head;
                while (previous->next != current)
                {
                    previous = previous->next;
                }
                previous->next = current->next;
            }
            BaseItem *item = current->item;
            delete current;
            countItem--;
            return item;
        }
        current = current->next;
    }
    return nullptr; // No usable item was found
}

BaseItem *BaseBag::get(ItemType item_type)
{
    Node *current = head;
    Node *previous = nullptr;
    while (current != nullptr)
    {
        if (current->item->getItemType() == item_type)
        {
            // Found the item, now remove it from the list
            if (previous == nullptr)
            {
                // The item is at the head of the list
                head = current->next;
            }
            else
            {
                // The item is somewhere in the middle or at the end of the list
                previous->next = current->next;
            }
            BaseItem *item = current->item;
            delete current;
            countItem--;
            return item;
        }
        previous = current;
        current = current->next;
    }
    return nullptr; // The item is not in the list
};

string BaseBag::str() const
{
    /*Bag[count=<c>;<list_items>]*/
    string res = "Bag[count=" + to_string(countItem) + ";";
    Node *temp = head;
    while (temp != nullptr)
    {
        res += to_string(temp->item->getItemType());
        if (temp->next != nullptr)
        {
            res += ";";
        }
        temp = temp->next;
    }
    return res + "]";
};

SherlockBag::SherlockBag(Sherlock *sherlock) : BaseBag(13)
{
    this->character = sherlock;
}

WatsonBag::WatsonBag(Watson *watson) : BaseBag(15)
{
    this->character = watson;
}

StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    ifstream ifs(config_file_path);
    if (!ifs.is_open())
    {
        return;
    }
    config = new Configuration(config_file_path);
    ifs.close();
}

bool StudyPinkProgram::isStop() const
{
    if (sherlock->getHP() == 1 || watson->getHP() == 1 || criminal->getCurrentPosition().isEqual(sherlock->getCurrentPosition()) || criminal->getCurrentPosition().isEqual(watson->getCurrentPosition()))
    {
        return true;
    }
    return false;
}

void StudyPinkProgram::run(bool verbose)
{
    // Note: This is a sample code. You can change the implementation as you like.
    // TODO
    for (int istep = 0; istep < config->num_steps; ++istep)
    {
        for (int i = 0; i < arr_mv_objs->size(); ++i)
        {
            arr_mv_objs->get(i)->move();
            int countSteps = 0;
            Robot *robot;
            if (arr_mv_objs->get(i)->getName() == "Criminal")
            {
                if (countSteps % 3 == 0)
                {
                    Robot *robot = Robot::create(3 + Robot::countRobots, map, criminal, sherlock, watson);
                }
                arr_mv_objs->get(i)->move();
                countSteps++;
            }
            if (arr_mv_objs->get(i)->getName() == "Sherlock")
            {
                sherlock->meetWatson(watson);
                sherlock->meetRobot(robot);
                sherlock->meetRobotC(robot);
            }
            if (arr_mv_objs->get(i)->getName() == "Watson")
            {
                watson->meetSherlock(sherlock);
                watson->meetRobot(robot);
                watson->meetRobotC(robot);
            }
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

int main()
{
    // create a test case for Configuration
    cout << "----- Testcase 123 -----" << endl;
    int num_walls = 8;
    Position arr_walls[] = {Position(1, 1), Position(2, 1), Position(3, 1), Position(4, 1), Position(1, 3), Position(2, 3), Position(3, 3), Position(4, 3)};
    int num_fake_walls = 2;
    Position arr_fake_walls[] = {Position(1, 2), Position(4, 2)};

    Map *map = new Map(5, 5, num_walls, arr_walls, num_fake_walls, arr_fake_walls);

    Watson *watson = new Watson(1, "DR", Position(0, 2), map, 100, 500);
    watson->move();
    cout << watson->str();

    delete map;
    delete watson;
}