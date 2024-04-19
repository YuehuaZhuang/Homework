// yz10746_hw13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>

using namespace std;

class Coordinate {
public:
    int x, y;
    Coordinate() {
        x = -1, y = -1;
    };
    Coordinate(int a, int b) {
        x = a, y = b;
    };
    bool compare(Coordinate other) {
        return (x == other.x) && (y == other.y);
    }
    bool operator<(const Coordinate& other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }
};

Coordinate randomMove(Coordinate coord) {
    int newx = coord.x;
    int newy = coord.y;
    int result = (rand() % 4);
    // 0左边，(x-1,y);   
    if (result == 0) {
        newx = coord.x - 1;
        newy = coord.y;
    }
    // 1右边，(x+1,y);
    else if (result == 1) {
        newx = coord.x + 1;
        newy = coord.y;
    }
    // 2上边(x,y+1);
    else if (result == 2) {
        newx = coord.x;
        newy = coord.y + 1;
    }
    // 3下边(x,y-1)
    else if (result == 3) {
        newx = coord.x;
        newy = coord.y - 1;
    }
    else {
        cout << "Error!";
    }
    Coordinate newCoord(newx, newy);
    return newCoord;
}

bool checkIfOccupied(Coordinate newCoordiate, vector<vector<int>> grid) {
    //0 代表没东西，1代表ant，2代表doodlebug
    if (grid[newCoordiate.x][newCoordiate.y] == 0) {
        return false;
    }
    else {
        return true;
    }
}

//检查新坐标有没有DB
bool checkIfDoodle(Coordinate newCoordiate, vector<vector<int>> grid) {
    //0 代表没东西，1代表ant，2代表doodlebug
    if (grid[newCoordiate.x][newCoordiate.y] == 0 || grid[newCoordiate.x][newCoordiate.y] == 1) {
        return false;
    }
    else {
        return true;
    }
}

//检查新坐标是否在grid外
bool checkIfOutside(Coordinate newCoordiate, Coordinate bounds) {
    if (newCoordiate.x >= 0 && newCoordiate.x < bounds.x &&
        newCoordiate.y >= 0 && newCoordiate.y < bounds.y) {
        return false;
    }
    else {
        return true;
    }
}

void printGrid(vector<vector<int>>& myGarden) {
    for (int i = 0;i < myGarden.size();i++) {
        for (int j = 0;j < myGarden[0].size();j++) {
            int val = myGarden[i][j];
            if (val == 0) {
                cout << "- ";
            }
            else if (val == 1) {
                cout << "o ";
            }
            else if (val == 2) {
                cout << "X ";
            }
        }
        cout << endl;
    }
}

Coordinate findAdjacent(Coordinate coord, vector<vector<int>> myGrid, int val = 1) {
    int x = coord.x;
    int y = coord.y;
    if (x + 1 <= 19 && myGrid[x + 1][y] == val) {
        return Coordinate(x + 1, y);
    }
    else if (x - 1 >= 0 && myGrid[x - 1][y] == val) {
        return Coordinate(x - 1, y);
    }
    else if (y + 1 <= 19 && myGrid[x][y + 1] == val) {
        return Coordinate(x, y + 1);;
    }
    else if (y - 1 >= 0 && myGrid[x][y - 1] == val) {
        return Coordinate(x, y - 1);
    }
    else {
        return Coordinate(-1, -1);
    }
}

Coordinate BOUND(20, 20);

class Organism {
public:
    Coordinate currentCoordinate;
    int breedCount;

    virtual void move(vector<vector<int>>& garden) {
        
    }  
    Organism() {
        breedCount = 0;
    }
    Organism (Coordinate inputCoordinate) {
        currentCoordinate.x = inputCoordinate.x;
        currentCoordinate.y = inputCoordinate.y;
        breedCount = 0;
    }

};

class Ant : public Organism {
public:
    Ant() { 
    }
    Ant (Coordinate inputCoordinate):Organism(inputCoordinate){
    }
    virtual void move(vector<vector<int>>& garden) override{
        Coordinate newpos = randomMove(currentCoordinate);
        if (!checkIfOutside(newpos, BOUND) && !checkIfOccupied(newpos, garden)) {
            garden.at(currentCoordinate.x).at(currentCoordinate.y) = 0;
            currentCoordinate = newpos;
            garden.at(newpos.x).at(newpos.y) = 1;
        }
    }
    void survive() {
        this->breedCount++;
    }
    bool checkIfBreed(vector<vector<int>>& garden) {
        if (breedCount >= 3) {
            Coordinate temp = findAdjacent(currentCoordinate, garden, 0);
            if (temp.x != -1 && temp.y != -1) {
                return true;
            }
        }
        return false;
    }
    virtual Ant breed(vector<vector<int>>& garden) {
        breedCount = 0;
        Coordinate newpos = findAdjacent(currentCoordinate, garden, 0);
        return Ant(newpos);
    }
};



class Doodlebug : public Organism {
public:
    int starveCount;
    Doodlebug() {
        starveCount = 0;
    }
    Doodlebug(Coordinate inputCoordinate) :Organism(inputCoordinate) {
        starveCount = 0;
    }
    void resetStarveCount() {
        this->starveCount = -1;
    }
    void survive() {
        this->breedCount++;
        this->starveCount++;
    }

    virtual void move(vector<vector<int>>& garden) override{
        Coordinate newpos = randomMove(currentCoordinate);
        if (!checkIfOutside(newpos, BOUND) && !checkIfOccupied(newpos, garden)) {
            garden.at(currentCoordinate.x).at(currentCoordinate.y) = 0;
            currentCoordinate = newpos;
            garden.at(newpos.x).at(newpos.y) = 2;
        }
    }

    bool tryEat(vector<Ant>& ants, vector<vector<int>>& garden) {
        if (ants.size() == 0) {
            return false;
        }
        Coordinate temp = findAdjacent(currentCoordinate, garden, 1);
        Coordinate newpos;
        if (temp.x != -1 && temp.y != -1) {//如果temp是个蚂蚁
            newpos = temp;//temp变为DB新坐标
            int indexToDelete = -1;
            for (int i = 0;i < ants.size();i++) {
                if (ants[i].currentCoordinate.x == temp.x && ants[i].currentCoordinate.y == temp.y) {
                    indexToDelete = i;
                    break;
                }
            }
            ants.erase(ants.begin() + indexToDelete);
            garden.at(currentCoordinate.x).at(currentCoordinate.y) = 0;//DB旧坐标清零
            currentCoordinate = newpos;//DB的坐标换成新的
            garden.at(newpos.x).at(newpos.y) = 2;//新坐标grid变为2
            resetStarveCount();//DB starvecount归零
            return true;
        }
        else {
            return false;
        }
    }

    bool checkIfBreed(vector<vector<int>>& garden) {
        if (breedCount >= 8) {
            Coordinate temp = findAdjacent(currentCoordinate, garden, 0);
            if (temp.x != -1 && temp.y != -1) {
                return true;
            }
        }
        return false;
    }
    virtual Doodlebug breed(vector<vector<int>>& garden) {
        breedCount = 0;
        Coordinate newpos = findAdjacent(currentCoordinate, garden, 0);
        return Doodlebug(newpos);
    }

    int tryDie(vector<Doodlebug>doodlebugs, vector<vector<int>>& garden) {
        if (starveCount == 3) {
            garden.at(currentCoordinate.x).at(currentCoordinate.y) = 0;
            for (int i = 0;i < doodlebugs.size();i++) {
                if (doodlebugs[i].currentCoordinate.x == currentCoordinate.x &&
                    doodlebugs[i].currentCoordinate.y == currentCoordinate.y) {
                    return i;
                }
            }
        }
        return -1;
    }
};
    
vector<Coordinate> generateCoordinates() {
    set<Coordinate> coordinates; 
    srand(time(NULL)); // Seed the random number generator
// Continue generating unique coordinates until we have 105
    while (coordinates.size() < 105) {
    int x = rand() % 20; // Generate x between 0 and 19
    int y = rand() % 20; // Generate y between 0 and 19
    coordinates.insert(Coordinate(x,y)); // std::set will automatically handle uniqueness
}
    return vector<Coordinate>(coordinates.begin(), coordinates.end());
}


int main(){
    //unsigned int boundx = static_cast<unsigned int>(BOUND.x);
    vector<vector<int>> myGarden(20, vector<int>(20));
    myGarden.at(0).at(0) = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0;j < 20;j++) {
            myGarden.at(i).at(j) = 0; 
        }
    }
    vector<Doodlebug> doodlebugs;
    vector<Ant> ants;
    vector<Coordinate> positionVec = generateCoordinates();
    for (int i = 0;i < 5;i++) {
        doodlebugs.push_back(Doodlebug(positionVec[i]));
    }
    for (int i = 5;i < positionVec.size();i++) {
        ants.push_back(Ant(positionVec[i]));
    }
    for (int i = 0;i < doodlebugs.size();i++) {
        myGarden.at(doodlebugs[i].currentCoordinate.x).at(doodlebugs[i].currentCoordinate.y) = 2;
    }
    for (int i = 0;i < ants.size();i++) {
        myGarden.at(ants[i].currentCoordinate.x).at(ants[i].currentCoordinate.y) = 1;
    }
    
    for (int i = 0;i < 100;i++) {
        cout << "Press ENTER to continue" << endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        for (int j = 0;j < doodlebugs.size();j++) {
            if (!doodlebugs[j].tryEat(ants, myGarden)) {
                doodlebugs[j].move(myGarden);
            }
            /*cout << "DB:" << doodlebugs[j].currentCoordinate.x << "," << doodlebugs[j].currentCoordinate.y << " ";*/
        }
        for (int j = 0;j < ants.size();j++) {
            ants[j].move(myGarden);
            //cout << "ANT:" << ants[j].currentCoordinate.x << "," << ants[j].currentCoordinate.y << " ";
        }
        vector<Ant>childAnts;
        for (int j = 0;j < ants.size();j++) {
            if (ants[j].checkIfBreed(myGarden)) {
                Ant childAnt = ants[j].breed(myGarden);
                childAnts.push_back(childAnt);
            }
            ants[j].survive();
        }
        ants.insert(ants.end(), childAnts.begin(), childAnts.end());
        vector<Doodlebug>childDoodlebugs;
        vector<int>doodlebugsToDieIndex;
        for (int j = 0;j < doodlebugs.size();j++) {
            if (doodlebugs[j].checkIfBreed(myGarden)) {
                Doodlebug childDoodlebug = doodlebugs[j].breed(myGarden);
                childDoodlebugs.push_back(childDoodlebug);
            }
            int index = doodlebugs[j].tryDie(doodlebugs, myGarden);
            doodlebugs[j].survive();
            if (index >= 0) {
                doodlebugsToDieIndex.push_back(index);
            }
        }
        for (int j = doodlebugsToDieIndex.size()-1;j >= 0;j--) {
            int index = doodlebugsToDieIndex[j];
            doodlebugs.erase(doodlebugs.begin() + index);
        }
        doodlebugs.insert(doodlebugs.end(), childDoodlebugs.begin(), childDoodlebugs.end());
        cout << "World at time " << i << endl;
        printGrid(myGarden);
        cout << endl;
    }

    
        
    

}

