#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include "jsoncpp/json.h"

const int MAX_WIDTH = 80;
const int MAX_HEIGHT = 40;

using namespace std;

struct Point;

vector<vector<Point> > points;

int pos[][2]=
{

    {
        -1,-1
    },
    {
        -1,0
    },
    {
        -1,1
    },
    {
        0,-1
    },
    {
        0,1
    },
    {
        1,-1
    },
    {
        1,0
    },
    {
        1,1
    }

};


struct Point
{
    int row;
    int col;
    int status;
    Point() {}

};
void write_to_stream(pair<int,int>p,string log)
{

    Json::Value output;
    output["response"]["row"] = p.first;
    output["response"]["col"] = p.second;
    output["debug"] = log;
    Json::FastWriter writer;
    cout << writer.write(output) << endl;
}
void write_to_stream(Point p,string log)
{
    Json::Value output;
    output["response"]["row"] = p.row;
    output["response"]["col"] = p.col;
    output["debug"] = log;
    Json::FastWriter writer;
    cout << writer.write(output) << endl;
}

void get_around(int row,int col)
{
    vector<Point>pts;
    for(int i=0; i<8; ++i)
    {
        int r=row+pos[i][0];
        int c=col+pos[i][1];
        if(r>=0&&r<points.size()&&c>=0&&c<points[0].size())
        {
            if(points[r][c].status>=0&&points[r][c].status<=8)
            {}
            else
            {
                pts.push_back(points[r][c]);
            }
        }
    }
    if(pts.size()==points[row][col].status)
    {
        for(auto &i:pts)
        {
            points[i.row][i.col].status = 9;
        }
    }

}



int main()
{
    srand(time(NULL));
    string str;
    getline(cin, str);
    Json::Reader reader;
    Json::Value input, output, lastInput;
    reader.parse(str, input);
    int len = input["requests"].size();
    lastInput = input["requests"][len - 1];
    const int fieldHeight = lastInput["height"].asInt();
    const int fieldWidth = lastInput["width"].asInt();
    points.resize(fieldHeight);
    for(auto &i:points)
    {
        i.resize(fieldWidth);
    }

    const int mineCount = lastInput["minecount"].asInt();
    for (int row = 0; row < fieldHeight; row++)
    {
        for (int col = 0; col < fieldWidth; col++)
        {
            points[row][col].status = 10;
            points[row][col].row = row;
            points[row][col].col = col;
        }
    }
    for (int i = 0; i < len; i++)
    {
        Json::Value changed = input["requests"][i]["changed"];
        if (changed.isArray())
        {
            int changedLen = changed.size();
            for (int j = 0; j < changedLen; j++)
            {
                points[changed[j]["row"].asInt()] [changed[j]["col"].asInt()].status = changed[j]["val"].asInt();
            }
        }
    }




    for (int row = 0; row < fieldHeight; row++)
    {
        for (int col = 0; col < fieldWidth; col++)
        {
            if(points[row][col].status>=1 && points[row][col].status<=8)
            {
                get_around(row,col);
            }
        }
    }
    /*

    for (int row = 0; row < fieldHeight; row++)
    {
        for (int col = 0; col < fieldWidth; col++)
        {
            if(ensure(row,col))
            {
                return 0;
            }
        }
    }
    */
    int unrevealedPos[MAX_HEIGHT * MAX_WIDTH][2], unrevealedCount = 0;
    for (int row = 0; row < fieldHeight; row++)
    {
        for (int col = 0; col < fieldWidth; col++)
        {
            if (points[row][col].status == 10)
            {
                unrevealedPos[unrevealedCount][0] = row;
                unrevealedPos[unrevealedCount][1] = col;
                unrevealedCount++;
            }
        }
    }
    if(unrevealedCount!=0)
    {
        int myChoice = rand() % unrevealedCount;
        int decidedRow = unrevealedPos[myChoice][0];
        int decidedCol = unrevealedPos[myChoice][1];
        write_to_stream(make_pair(decidedRow,decidedCol),"2");
    }
}
