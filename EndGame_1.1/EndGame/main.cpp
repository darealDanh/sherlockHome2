/*
 * Hướng dẫn chạy testcase
 * Chạy 2 lệnh sau trong terminal
 *
 * Lệnh 1:
 * g++ -o main main.cpp fight.cpp
 *
 * Lệnh 2:
 * Chạy full 1000 test
 * ./main
 *
 * Chạy test chỉ định
 * ./main <test>
 * VD: ./main 111
 *
 * Chạy cụm test
 * ./main <from> <to>
 * VD: ./main 1 10
 */

#include "study_in_pink21.h"
#include "vector"
using namespace std;

void comparefile(int start, int end)
{
    vector<int> result;
    for (int i = start; i <= end; i++)
    {
        string outputFile = "testcase/output/output" + to_string(i) + ".txt";
        string expectFile = "testcase/expect/expect" + to_string(i) + ".txt";
        ifstream read_output(outputFile);
        ifstream read_expect(expectFile);
        if (read_expect.fail())
        {
            cout << "Error reading file - " << i << endl;
            return;
        }
        string s1, s2;
        while (read_output >> s1 && read_expect >> s2)
        {
            if (s1 != s2)
            {
                result.push_back(i);
                break;
            }
        }
        if (read_output >> s1 || read_expect >> s2)
        {
            if (result.size() == 0 || result.size() > 0 && result[result.size() - 1] != i)
                result.push_back(i);
        }
    }

    if (result.size() == 0)
    {
        cout << "Success: test " << start << " to " << end << endl;
    }
    else
    {
        cout << "percent success : " << (1 - result.size() * 1.0 / (end - start + 1)) * 100 << "%" << endl;
        cout << "Fail : test [";
        for (int i = 0; i < result.size() - 1; i++)
        {
            cout << result[i] << ", ";
        }
        cout << result[result.size() - 1] << "]\n";
        cout << "link check comparefile: https://www.diffchecker.com/text-compare/" << endl;
    }
}
class TestStudyInPink
{
public:
    static void testBasic()
    {
        // MapElement
        MapElement *testMapElement = new Path();
        OUTPUT << "Path getType: " << testMapElement->getType() << endl;
        delete testMapElement;
        testMapElement = new Wall();
        OUTPUT << "Wall getType: " << testMapElement->getType() << endl;
        delete testMapElement;
        testMapElement = new FakeWall(666);
        OUTPUT << "FakeWall getType: " << testMapElement->getType() << endl;
        delete testMapElement;
        // Map
        Position arrayWall[1] = {Position(0, 1)};
        Position arrayFakeWall[1] = {Position(0, 0)};
        Map *testMap = new Map(1, 3, 1, arrayWall, 1, arrayFakeWall);
        // isValid Watson
        MovingObject *dummyObj = new Watson(0, "", Position(0, 0), nullptr, 0, 0);
        OUTPUT << "Watson go in Path: " << testMap->isValid(Position(0, 2), dummyObj) << endl;
        OUTPUT << "Watson go in Wall: " << testMap->isValid(Position(0, 1), dummyObj) << endl;
        OUTPUT << "Watson go in FakeWall with EXP_" << dynamic_cast<Watson *>(dummyObj)->getEXP() << " : " << testMap->isValid(Position(0, 0), dummyObj) << endl;
        dynamic_cast<Watson *>(dummyObj)->setEXP(89);
        OUTPUT << "Watson go in FakeWall with EXP_" << dynamic_cast<Watson *>(dummyObj)->getEXP() << " : " << testMap->isValid(Position(0, 0), dummyObj) << endl;
        dynamic_cast<Watson *>(dummyObj)->setEXP(90);
        OUTPUT << "Watson go in FakeWall with EXP_" << dynamic_cast<Watson *>(dummyObj)->getEXP() << " : " << testMap->isValid(Position(0, 0), dummyObj) << endl;
        delete dummyObj;
        // isValid Sherlock
        dummyObj = new Sherlock(0, "", Position(0, 0), nullptr, 0, 0);
        OUTPUT << "Sherlock go in Path: " << testMap->isValid(Position(0, 2), dummyObj) << endl;
        OUTPUT << "Sherlock go in Wall: " << testMap->isValid(Position(0, 1), dummyObj) << endl;
        OUTPUT << "Sherlock go in FakeWall: " << testMap->isValid(Position(0, 0), dummyObj) << endl;
        delete dummyObj;
        // isValid Criminal
        dummyObj = new Criminal(0, Position(0, 0), nullptr, nullptr, nullptr);
        OUTPUT << "Criminal go in Path: " << testMap->isValid(Position(0, 2), dummyObj) << endl;
        OUTPUT << "Criminal go in Wall: " << testMap->isValid(Position(0, 1), dummyObj) << endl;
        OUTPUT << "Criminal go in FakeWall: " << testMap->isValid(Position(0, 0), dummyObj) << endl;
        delete dummyObj;
        // isValid Robot
        dummyObj = new RobotC(0, Position(0, 0), nullptr, nullptr);
        OUTPUT << "Robot go in Path: " << testMap->isValid(Position(0, 2), dummyObj) << endl;
        OUTPUT << "Robot go in Wall: " << testMap->isValid(Position(0, 1), dummyObj) << endl;
        OUTPUT << "Robot go in FakeWall: " << testMap->isValid(Position(0, 0), dummyObj) << endl;
        // isValid outrange
        OUTPUT << "Outrange " << Position(-1, 0).str() << " : " << testMap->isValid(Position(-1, 0), dummyObj) << endl;
        OUTPUT << "Outrange " << Position(0, -1).str() << " : " << testMap->isValid(Position(0, -1), dummyObj) << endl;
        OUTPUT << "Outrange " << Position(1, 0).str() << " : " << testMap->isValid(Position(1, 0), dummyObj) << endl;
        OUTPUT << "Outrange " << Position(0, 3).str() << " : " << testMap->isValid(Position(0, 3), dummyObj) << endl;
        delete dummyObj;
        delete testMap;
        // Position
        OUTPUT << "Position: " << Position().str() << endl;
        OUTPUT << "Position (1,1): " << Position(1, 1).str() << endl;
        OUTPUT << "Position String (2,2): " << Position("(2,2)").str() << endl;
        OUTPUT << "Position getRow: " << Position().getRow() << endl;
        OUTPUT << "Position getCol: " << Position().getCol() << endl;
        Position testPos;
        testPos.setRow(1);
        testPos.setCol(1);
        OUTPUT << "Position setRow: " << testPos.getRow() << endl;
        OUTPUT << "Position setCol: " << testPos.getCol() << endl;
        OUTPUT << "Position (0,0) isEqual (0,0): " << Position(0, 0).isEqual(Position(0, 0)) << endl;
        OUTPUT << "Position (0,0) isEqual (0,1): " << Position(0, 0).isEqual(Position(0, 1)) << endl;
        OUTPUT << "Position (0,0) isEqual (0,0): " << Position(0, 0).isEqual(0, 0) << endl;
        OUTPUT << "Position (0,0) isEqual (0,1): " << Position(0, 0).isEqual(0, 1) << endl;
        // MovingObject
        Map dummyMap(10, 10, 0, nullptr, 0, nullptr);
        MovingObject *sherlock = new Sherlock(1, "URDL", Position(0, 0), &dummyMap, 0, 0);
        MovingObject *watson = new Watson(2, "URDL", Position(0, 0), &dummyMap, 0, 0);
        MovingObject *criminal = new Criminal(0, Position(0, 0), &dummyMap, dynamic_cast<Sherlock *>(sherlock), dynamic_cast<Watson *>(watson));
        MovingObject *robotC = new RobotC(3, Position(5, 5), &dummyMap, dynamic_cast<Criminal *>(criminal));
        MovingObject *robotS = new RobotS(4, Position(5, 5), &dummyMap, dynamic_cast<Criminal *>(criminal), dynamic_cast<Sherlock *>(sherlock));
        MovingObject *robotW = new RobotW(5, Position(5, 5), &dummyMap, dynamic_cast<Criminal *>(criminal), dynamic_cast<Watson *>(watson));
        MovingObject *robotSW = new RobotSW(6, Position(5, 5), &dummyMap, dynamic_cast<Criminal *>(criminal), dynamic_cast<Sherlock *>(sherlock), dynamic_cast<Watson *>(watson));
        // str
        OUTPUT << "Sherlock: " << sherlock->str() << endl;
        OUTPUT << "Watson: " << watson->str() << endl;
        OUTPUT << "Criminal: " << criminal->str() << endl;
        OUTPUT << "RobotC: " << robotC->str() << endl;
        OUTPUT << "RobotS: " << robotS->str() << endl;
        OUTPUT << "RobotW: " << robotW->str() << endl;
        OUTPUT << "RobotSW: " << robotSW->str() << endl;
        // getCurrentPosition
        OUTPUT << "Sherlock: " << sherlock->getCurrentPosition().str() << endl;
        OUTPUT << "Watson: " << watson->getCurrentPosition().str() << endl;
        OUTPUT << "Criminal: " << criminal->getCurrentPosition().str() << endl;
        OUTPUT << "RobotC: " << robotC->getCurrentPosition().str() << endl;
        OUTPUT << "RobotS: " << robotS->getCurrentPosition().str() << endl;
        OUTPUT << "RobotW: " << robotW->getCurrentPosition().str() << endl;
        OUTPUT << "RobotSW: " << robotSW->getCurrentPosition().str() << endl;
        // getNextPosition
        // move
        for (int imove = 0; imove < 10; imove++)
        {
            OUTPUT << "Sherlock getNextPosition: " << sherlock->getNextPosition().str() << endl;
            OUTPUT << "Watson getNextPosition: " << watson->getNextPosition().str() << endl;
            OUTPUT << "Criminal getNextPosition: " << criminal->getNextPosition().str() << endl;
            OUTPUT << "RobotC getNextPosition: " << robotC->getNextPosition().str() << endl;
            OUTPUT << "RobotS getNextPosition: " << robotS->getNextPosition().str() << endl;
            OUTPUT << "RobotW getNextPosition: " << robotW->getNextPosition().str() << endl;
            OUTPUT << "RobotSW getNextPosition: " << robotSW->getNextPosition().str() << endl;

            sherlock->move();
            watson->move();
            criminal->move();
            robotC->move();
            robotS->move();
            robotW->move();
            robotSW->move();

            OUTPUT << "Sherlock move: " << sherlock->str() << endl;
            OUTPUT << "Watson move: " << watson->str() << endl;
            OUTPUT << "Criminal move: " << criminal->str() << endl;
            OUTPUT << "RobotC move: " << robotC->str() << endl;
            OUTPUT << "RobotS move: " << robotS->str() << endl;
            OUTPUT << "RobotW move: " << robotW->str() << endl;
            OUTPUT << "RobotSW move: " << robotSW->str() << endl;

            OUTPUT << "RobotC distance Sherlock: " << dynamic_cast<RobotC *>(robotC)->getDistance(dynamic_cast<Sherlock *>(sherlock)) << endl;
            OUTPUT << "RobotC distance Watson: " << dynamic_cast<RobotC *>(robotC)->getDistance(dynamic_cast<Watson *>(watson)) << endl;
            OUTPUT << "RobotS distance: " << dynamic_cast<Robot *>(robotS)->getDistance() << endl;
            OUTPUT << "RobotW distance: " << dynamic_cast<Robot *>(robotW)->getDistance() << endl;
            OUTPUT << "RobotSW distance: " << dynamic_cast<Robot *>(robotSW)->getDistance() << endl;
        }
        // ArrayMovingObject
        // add
        // isFull
        // str
        ArrayMovingObject *testArray = new ArrayMovingObject(7);
        OUTPUT << "isFull: " << testArray->isFull() << endl;
        OUTPUT << "ArrayMovingObject str: " << testArray->str() << endl;
        OUTPUT << "ArrayMovingObject add: " << testArray->add(criminal) << endl;
        OUTPUT << "isFull: " << testArray->isFull() << endl;
        OUTPUT << "ArrayMovingObject str: " << testArray->str() << endl;
        OUTPUT << "ArrayMovingObject add: " << testArray->add(sherlock) << endl;
        OUTPUT << "isFull: " << testArray->isFull() << endl;
        OUTPUT << "ArrayMovingObject str: " << testArray->str() << endl;
        OUTPUT << "ArrayMovingObject add: " << testArray->add(watson) << endl;
        OUTPUT << "isFull: " << testArray->isFull() << endl;
        OUTPUT << "ArrayMovingObject str: " << testArray->str() << endl;
        OUTPUT << "ArrayMovingObject add: " << testArray->add(robotC) << endl;
        OUTPUT << "isFull: " << testArray->isFull() << endl;
        OUTPUT << "ArrayMovingObject str: " << testArray->str() << endl;
        OUTPUT << "ArrayMovingObject add: " << testArray->add(robotS) << endl;
        OUTPUT << "isFull: " << testArray->isFull() << endl;
        OUTPUT << "ArrayMovingObject str: " << testArray->str() << endl;
        OUTPUT << "ArrayMovingObject add: " << testArray->add(robotW) << endl;
        OUTPUT << "isFull: " << testArray->isFull() << endl;
        OUTPUT << "ArrayMovingObject str: " << testArray->str() << endl;
        OUTPUT << "ArrayMovingObject add: " << testArray->add(robotSW) << endl;
        OUTPUT << "isFull: " << testArray->isFull() << endl;
        OUTPUT << "ArrayMovingObject str: " << testArray->str() << endl;
        OUTPUT << "ArrayMovingObject add: " << testArray->add(criminal) << endl;
        OUTPUT << "isFull: " << testArray->isFull() << endl;
        OUTPUT << "ArrayMovingObject str: " << testArray->str() << endl;
        delete testArray;
        delete sherlock;
        delete watson;
        delete criminal;
    }
    static void testAll(string inputFile, string outputFile)
    {
        StudyPinkProgram testStudyPinkProgram(inputFile);
        testStudyPinkProgram.setOutputFile(outputFile);
        testStudyPinkProgram.run(true);
    }
};

int main(int argc, char *argv[])
{
    cout << "Start program assignments 2 KTLT" << endl;
    cout << "Running test : ";
    int start = 0, end = 0;
    if (argc == 1)
    {
        start = 0;
        end = 10000;
    }
    else if (argc == 2)
    {
        start = stoi(argv[1]);
        end = stoi(argv[1]);
    }
    else if (argc == 3)
    {
        start = stoi(argv[1]);
        end = stoi(argv[2]);
    }
    string folderr = "testcase/output/output";
    // string folderr = "testcase/expect/expect";
    if (start == 0)
    {
        string outputFile = folderr + "0.txt";
        OUTPUT.open(outputFile);
        TestStudyInPink::testBasic();
        OUTPUT.close();
        start = 1;
    }
    for (int i = start; i <= end; i++)
    {
        cout << i << " ";
        string inputFile = "testcase/input/input" + to_string(i) + ".txt";
        string outputFile = folderr + to_string(i) + ".txt";
        OUTPUT.open(outputFile);
        TestStudyInPink::testAll(inputFile, outputFile);
        OUTPUT.close();
    }
    cout << endl;
    comparefile(start, end);
}
