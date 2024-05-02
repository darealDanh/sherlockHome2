#include "game.h"

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

MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
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

void MovingObject::setExp(int exp)
{
    this->exp = exp;
}

int MovingObject::getHP() const
{
    return hp;
}

int MovingObject::getExp() const
{
    return exp;
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
        if (mv_obj->getName() == "Sherlock" || mv_obj->getName() == "Criminal" || mv_obj->getName() == "RobotC" || mv_obj->getName() == "RobotS" || mv_obj->getName() == "RobotW" || mv_obj->getName() == "RobotSW")
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
    sscanf(str_pos.c_str(), "(%d,%d)", &r, &c);
}

bool Position::isEqual(const Position &pos) const
{
    if (r == pos.getRow() && c == pos.getCol())
        return true;
    return false;
}

Character::Character(int index, const Position &init_pos, Map *map, const string &name) : MovingObject(index, init_pos, map, name)
{
    // finish this constructor
    this->index = index;
    this->pos = init_pos;
    this->map = map;
    this->name = name;
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
            next_pos = Position::npos;
        }
        i++;
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

Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, pos, map, "Sherlock")
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
    if (pos.isEqual(watson->getCurrentPosition()))
    {
        meetWatson(watson);
    }
    if (pos.isEqual(criminal->getCurrentPosition()))
    {
        meetCriminal(criminal);
    }
    if (pos.isEqual(robot->getCurrentPosition()))
    {
        if (this->bag->get(ItemType::PASSING_CARD) != nullptr)
        {
            BaseItem *item = this->bag->get(ItemType::PASSING_CARD);
            item->use(this, robot);
        }
        else
        {
            meetRobot(robot);
            // if sherlock has magicbook, firstaid or energydrink, use it now
            ItemType item = ItemType::MAGIC_BOOK;
            BaseItem *magicbook = this->bag->get(item);
            while (magicbook != nullptr)
            {
                magicbook->use(this, robot);
                magicbook = this->bag->get(item);
            }
            item = ItemType::FIRST_AID;
            BaseItem *firstaid = this->bag->get(item);
            while (firstaid != nullptr)
            {
                firstaid->use(this, robot);
                firstaid = this->bag->get(item);
            }
            item = ItemType::ENERGY_DRINK;
            BaseItem *energydrink = this->bag->get(item);
            while (energydrink != nullptr)
            {
                energydrink->use(this, robot);
                energydrink = this->bag->get(item);
            }
        }
    }
    if (pos.isEqual(robotC->getCurrentPosition()))
    {
        if (this->bag->get(ItemType::PASSING_CARD) != nullptr)
        {
            BaseItem *item = this->bag->get(ItemType::PASSING_CARD);
            item->use(this, robotC);
        }
        else
        {
            meetRobotC(robotC, criminal);
            // if sherlock has magicbook, firstaid or energydrink, use it now
            ItemType item = ItemType::MAGIC_BOOK;
            BaseItem *magicbook = this->bag->get(item);
            while (magicbook != nullptr)
            {
                magicbook->use(this, robotC);
                magicbook = this->bag->get(item);
            }
            item = ItemType::FIRST_AID;
            BaseItem *firstaid = this->bag->get(item);
            while (firstaid != nullptr)
            {
                firstaid->use(this, robotC);
                firstaid = this->bag->get(item);
            }
            item = ItemType::ENERGY_DRINK;
            BaseItem *energydrink = this->bag->get(item);
            while (energydrink != nullptr)
            {
                energydrink->use(this, robotC);
                energydrink = this->bag->get(item);
            }
        }
    }
}

void Sherlock::meetRobotC(RobotC *robotC, Criminal *criminal)
{
    if (this->getExp() > 500)
    {
        this->meetCriminal(criminal);
    }
    else
    {
        BaseItem *item = robotC->getItem();
        this->bag->insert(item);
    }
}

void Sherlock::meetRobot(Robot *robot)
{
    if (robot->getRobotType() == 1)
    {
        if (this->getExp() > 400)
        {
            BaseItem *item = robot->getItem();
            this->bag->insert(item);
        }
        else
        {
            this->setExp(this->getExp() * 0.9);
        }
    }
    if (robot->getRobotType() == 2)
    {
        BaseItem *item = robot->getItem();
        this->bag->insert(item);
    }
    if (robot->getRobotType() == 3)
    {
        if (this->getExp() > 300 && this->getHP() > 335)
        {
            BaseItem *item = robot->getItem();
            this->bag->insert(item);
        }
        else
        {
            this->setExp(this->getExp() * 0.85);
            this->setHP(this->getHP() * 0.85);
        }
    }
}

void Sherlock::meetWatson(Watson *watson)
{
    // check if there are any passing card in sherlockBag;
    ItemType item = ItemType::PASSING_CARD;
    BaseItem *pcard = this->bag->get(item);
    while (pcard != nullptr)
    {
        watson->getBag()->insert(pcard);
        pcard = this->bag->get(item);
    }
}

void Sherlock::meetCriminal(Criminal *criminal)
{
    this->getNextPosition() = criminal->getCurrentPosition();
}

Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson")
{
    // finish the constructor
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

void Watson::meetSherlock(Sherlock *sherlock)
{
    // check if there are any exemcemption card in watsonBag;
    ItemType item = ItemType::EXCEMPTION_CARD;
    BaseItem *ecard = this->bag->get(item);
    while (ecard != nullptr)
    {
        sherlock->getBag()->insert(ecard);
        ecard = this->bag->get(item);
    }
}

void Watson::meetRobotC(RobotC *robotC, Criminal *criminal)
{
    BaseItem *item = robotC->getItem();
    this->bag->insert(item);
}

void Watson::meetRobot(Robot *robot)
{
    if (robot->getRobotType() == 2)
    {
        if (this->getHP() > 350)
        {
            BaseItem *item = robot->getItem();
            this->bag->insert(item);
        }
        else
        {
            this->setHP(this->getHP() * 0.95);
        }
    }
    if (robot->getRobotType() == 3)
    {
        if (this->getExp() > 600 && this->getHP() > 165)
        {
            BaseItem *item = robot->getItem();
            this->bag->insert(item);
        }
        else
        {
            this->setExp(this->getExp() * 0.85);
            this->setHP(this->getHP() * 0.85);
        }
    }
}

void Watson::meetCriminal(Criminal *criminal)
{
    pos = criminal->getCurrentPosition();
}

void Watson::move()
{
    Position next_pos = getNextPosition();
    if (&next_pos == &Position::npos)
    {
        return;
    }
    pos = next_pos;
    if (pos.isEqual(sherlock->getCurrentPosition()))
    {
        meetSherlock(sherlock);
    }
    if (pos.isEqual(criminal->getCurrentPosition()))
    {
        meetCriminal(criminal);
    }
    if (pos.isEqual(robot->getCurrentPosition()))
    {
        if (this->bag->get(ItemType::EXCEMPTION_CARD) != nullptr)
        {
            BaseItem *item = this->bag->get(ItemType::EXCEMPTION_CARD);
            item->use(this, robot);
        }
        else
        {
            meetRobot(robot);
            // if watson has magicbook, firstaid or energydrink, use it now
            ItemType item = ItemType::MAGIC_BOOK;
            BaseItem *magicbook = this->bag->get(item);
            while (magicbook != nullptr)
            {
                magicbook->use(this, robot);
                magicbook = this->bag->get(item);
            }
            item = ItemType::FIRST_AID;
            BaseItem *firstaid = this->bag->get(item);
            while (firstaid != nullptr)
            {
                firstaid->use(this, robot);
                firstaid = this->bag->get(item);
            }
            item = ItemType::ENERGY_DRINK;
            BaseItem *energydrink = this->bag->get(item);
            while (energydrink != nullptr)
            {
                energydrink->use(this, robot);
                energydrink = this->bag->get(item);
            }
        }
    }
    if (pos.isEqual(robotC->getCurrentPosition()))
    {
        if (this->bag->get(ItemType::EXCEMPTION_CARD) != nullptr)
        {
            BaseItem *item = this->bag->get(ItemType::EXCEMPTION_CARD);
            item->use(this, robotC);
        }
        else
        {
            meetRobotC(robotC, criminal);
            // if watson has magicbook, firstaid or energydrink, use it now
            ItemType item = ItemType::MAGIC_BOOK;
            BaseItem *magicbook = this->bag->get(item);
            while (magicbook != nullptr)
            {
                magicbook->use(this, robotC);
                magicbook = this->bag->get(item);
            }
            item = ItemType::FIRST_AID;
            BaseItem *firstaid = this->bag->get(item);
            while (firstaid != nullptr)
            {
                firstaid->use(this, robotC);
                firstaid = this->bag->get(item);
            }
            item = ItemType::ENERGY_DRINK;
            BaseItem *energydrink = this->bag->get(item);
            while (energydrink != nullptr)
            {
                energydrink->use(this, robotC);
                energydrink = this->bag->get(item);
            }
        }
    };
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
    if (&next_pos == &pos)
    {
        next_pos = Position::npos;
    }
    return next_pos;
}

void Criminal::move()
{
    if (countSteps == 3)
    {
        /*Nếu là robot đầu tiên được tạo ra trên bản đồ, đó sẽ là loại robot RobotC. Nếu không,
ta xét khoảng cách từ Robot đến Sherlock và Watson:
• Nếu khoảng cách đến Sherlock gần hơn: Tạo ra loại robot RobotS
• Khoảng cách đến Watson gần hơn: Tạo ra loại robot RobotW
• Khoảng cách đến Sherlock và Watson là bằng nhau: Tạo ra loại robot RobotSW*/
        if (Robot::countRobots == 0)
        {
            *robot = RobotC(0, pos, map, this);
        }
        else
        {
            int distanceS = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
            int distanceW = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
            if (distanceS < distanceW)
            {
                *robot = RobotS(0, pos, map, this, sherlock);
            }
            else if (distanceS > distanceW)
            {
                *robot = RobotW(0, pos, map, this, watson);
            }
            else
            {
                *robot = RobotSW(0, pos, map, this, sherlock, watson);
            }
        }
        countSteps = 0;
    }
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

ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    this->count = 0;
    // create an new array of MovingObject with the size of capacity
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

int ArrayMovingObject::size() const
{
    return count;
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

Robot::Robot(int index, const Position &init_pos, Map *map, RobotType robot_type) : MovingObject(index, init_pos, map, "Robot")

{
    this->robot_type = robot_type;
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
        *item = ExemptionCard();
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

int Robot::countRobots = 0;

BaseItem *Robot::getItem()
{
    return item;
}

RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal) : Robot(index, init_pos, map, C)
{
    this->criminal = criminal;
    countRobots++;
    this->name = "RobotC";
};

Position RobotC::getNextPosition()
{
    // go to next_pos with criminal
    Position next_pos = criminal->getCurrentPosition();
    if (&next_pos == &pos)
    {
        next_pos = Position::npos;
    }
    return next_pos;
};

string RobotC::str() const
{
    // Robot[pos=<pos>;type=<robot_type>;dist=<empty>]
    return "Robot[pos=" + pos.str() + ";type=" + to_string(robot_type) + ";dist=]";
}

int RobotC::getDistance(Character *character) const
{
    /*Riêng robot loại C cung cấp 2 phương thức tính toán khoảng cách đến
Sherlock hoặc đến Watson dựa trên 1 tham số đầu vào là con trỏ Sherlock* sherlock
hay Watson* watson.*/
    return abs(pos.getRow() - character->getCurrentPosition().getRow()) + abs(pos.getCol() - character->getCurrentPosition().getCol());
};

RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *Sherlock) : Robot(index, init_pos, map, S)
{
    this->sherlock = Sherlock;
    this->criminal = criminal;
    this->name = "RobotS";
    countRobots++;
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
    return "Robot[pos=" + pos.str() + ";type=" + to_string(robot_type) + ";dist=" + to_string(getDistance()) + "]";
}

RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson) : Robot(index, init_pos, map, W)
{
    this->watson = watson;
    this->criminal = criminal;
    this->name = "RobotW";
    countRobots++;
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
    return "Robot[pos=" + pos.str() + ";type=" + to_string(robot_type) + ";dist=" + to_string(getDistance()) + "]";
}

RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *Sherlock, Watson *watson) : Robot(index, init_pos, map, SW)
{
    this->sherlock = Sherlock;
    this->watson = watson;
    this->criminal = criminal;
    this->name = "RobotSW";
    countRobots++;
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
    return "Robot[pos=" + pos.str() + ";type=" + to_string(robot_type) + ";dist=" + to_string(getDistance()) + "]";
}

BaseItem::BaseItem(ItemType item_type, int value)
{
    this->item_type = item_type;
    this->value = value;
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

ItemType BaseItem::getItemType() const
{
    return item_type;
}

MagicBook::MagicBook() : BaseItem(ItemType::MAGIC_BOOK, 0)
{
}

bool MagicBook::canUse(Character *obj, Robot *robot)
{
    if (obj->getExp() >= 350)
    {
        return true;
    }
    return false;
}

void MagicBook::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        obj->setExp(obj->getExp() * 1.25);
    }
}

EnergyDrink::EnergyDrink() : BaseItem(ItemType::ENERGY_DRINK, 0)
{
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

FirstAid::FirstAid() : BaseItem(ItemType::FIRST_AID, 0)
{
}

bool FirstAid::canUse(Character *obj, Robot *robot)
{
    if (obj->getHP() <= 100 || obj->getExp() <= 350)
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

ExemptionCard::ExemptionCard() : BaseItem(ItemType::EXCEMPTION_CARD, 0)
{
}

bool ExemptionCard::canUse(Character *obj, Robot *robot)
{
    if (obj->getName() == "Sherlock" && obj->getHP() % 2 != 0)
    {
        return true;
    }
    return false;
}

void ExemptionCard::use(Character *obj, Robot *robot)
{
    obj->setExp(obj->getExp());
    obj->setHP(obj->getHP());
}

PassingCard::PassingCard(const string &challenges) : BaseItem(ItemType::PASSING_CARD, 0)
{
    this->challenges = challenges;
}

bool PassingCard::canUse(Character *obj, Robot *robot)
{
    if (obj->getName() == "Watson" && obj->getExp() % 2 != 0)
    {
        return true;
    }
    return false;
}

void PassingCard::use(Character *obj, Robot *robot)
{
    if (challenges == robot->getName())
    {
        BaseItem *item = robot->getItem();
        obj->getBag()->insert(item);
    }
    else
    {
        obj->setExp(obj->getExp() - 50);
        BaseItem *item = robot->getItem();
        obj->getBag()->insert(item);
    }
}

BaseBag::BaseBag(Character *character)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->countItem = 0;
}

BaseBag::~BaseBag()
{
    while (head != nullptr)
    {
        BaseItem *temp = head;
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
            if (head == nullptr)
            {
                head = item;
                tail = item;
            }
            else
            {
                item->next = head;
                head = item;
            }
        }
        else
            return false;
    }
    else if (character->getName() == "Watson")
    {
        if (countItem <= 15)
        {
            if (head == nullptr)
            {
                head = item;
                tail = item;
            }
            else
            {
                item->next = head;
                head = item;
            }
        }
        else
            return false;
    }
    else
        return false;
    countItem++;
    return true;
};

BaseItem *BaseBag::get()
{
    BaseItem *temp = head;
    while (temp != nullptr)
    {
        if (temp->canUse(character, nullptr))
        {
            BaseItem *temp2 = head;
            head = temp;
            temp = temp2;
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
};

BaseItem *BaseBag::get(ItemType item_type)
{
    BaseItem *temp = head;
    while (temp != nullptr)
    {
        if (temp->getItemType() == item_type)
        {
            if (temp == head)
            {
                BaseItem *result = temp;
                delete temp;
                return result;
            }
            else
            {
                // switch the temp with the head, return the temp and delete temp
                BaseItem *temp2 = head;
                head = temp;
                temp = temp2;
                BaseItem *result = temp;
                delete temp;
                return result;
            }
        }
        temp = temp->next;
    }
    return nullptr;
};

string BaseBag::str() const
{
    /*Bag[count=<c>;<list_items>]*/
    string res = "Bag[count=" + to_string(countItem) + ";";
    BaseItem *temp = head;
    while (temp != nullptr)
    {
        res += to_string(temp->getItemType()) + ";";
        temp = temp->next;
    }
};

SherlockBag::SherlockBag(Sherlock *sherlock) : BaseBag(sherlock)
{
    this->character = sherlock;
}

WatsonBag::WatsonBag(Watson *watson) : BaseBag(watson)
{
    this->character = watson;
}

StudyInPinkProgram::StudyInPinkProgram(const string &filepath)
{
    ifstream ifs(filepath);
    if (!ifs.is_open())
    {
        return;
    }
    config = new Configuration(filepath);
    ifs.close();
}

bool StudyInPinkProgram::isStop() const
{
    if (sherlock->getHP() == 0 || watson->getHP() == 0 || criminal->getCurrentPosition().isEqual(sherlock->getCurrentPosition()) || criminal->getCurrentPosition().isEqual(watson->getCurrentPosition()))
    {
        return true;
    }
    return false;
}

int main()
{
    // create an example test case for sherlock watson, a map, a criminal and a robot
    Position sherlock_init_pos(1, 3);
    Position watson_init_pos(2, 1);
    Position criminal_init_pos(7, 9);
    Position robot_init_pos(3, 4);
    Position arr_walls[3] = {Position(1, 2), Position(2, 3), Position(3, 4)};
    Position arr_fake_walls[1] = {Position(4, 5)};
    Map map(10, 10, 3, arr_walls, 1, arr_fake_walls);
    ArrayMovingObject arr_mv_objs(2);
    Sherlock sherlock(0, "RUU", sherlock_init_pos, &map, 250, 500);
    Watson watson(1, "LU", watson_init_pos, &map, 300, 350);
    Criminal criminal(0, criminal_init_pos, &map, &sherlock, &watson);
    arr_mv_objs.add(&sherlock);
    arr_mv_objs.add(&watson);
    arr_mv_objs.add(&criminal);
    // create an example scenario
    sherlock.move();
    watson.move();
    criminal.move();
    sherlock.move();
    watson.move();
    criminal.move();
    sherlock.move();
    watson.move();
    criminal.move();
    return 0;
}