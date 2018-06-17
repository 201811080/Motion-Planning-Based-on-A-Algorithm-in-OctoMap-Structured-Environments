#include <iostream>
#include <stdexcept>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <chrono>

#include "Astar.h"

int PERCENTAGE = 10;

inline int SlucajniBroj (int mini, int maxi){
    int n = maxi - mini + 1;
    int ost = RAND_MAX % n;
    int x;
    do{
        x = rand();
    }while (x >= RAND_MAX - ost);
    return mini + x % n;
}

void generateMatrix(int matrix[], int width, int height){
    int occupied = width * height / PERCENTAGE;
    for(int i{}; i<occupied; i++){
        int x = SlucajniBroj(0, width-1);
        int y = SlucajniBroj(0, height-1);
        //std::cout << x << " " << y << std::endl;
        if(!matrix[y*width + x])
            matrix[y*width + x] = 1;
        else
            i--;
    }
}

void generateFullMatrix(int matrix[], int width, int height){
    for(int i{}; i<width*height; i++){
            matrix[i] = 1;
    }
}

void loadMatrix(std::string ime, int matrix[], int width, int height){
    std::ifstream file;
    file.open(ime);
    char a;
    int i = 0;
    file >> a;
    while(!file.eof()){
        if(a != ' ' && a != ','){
            matrix[i] = a - '0';
            i++;
        }
        file >> a;
    }
}

void generateTxt(std::string ime, int matrix[], int width, int height){
    std::ofstream izlaz(ime);
    for(int i{}; i<width; i++){
        for(int j{}; j<height; j++)
            izlaz << matrix[j*width + i];
        izlaz << std::endl;
    }
}

bool compareFiles(std::string prvi, std::string drugi){
    std::ifstream file1, file2;
    file1.open(prvi);
    file2.open(drugi);
    char a,b;
    while(!file1.eof() && !file2.eof()){
        file1 >> a;
        file2 >> b;
        if(a != b)
            return false;
    }
    return true;
}

void setDepth(int width, int height, double tolerance){
    DEPTH = 2;
    while(width/2. > tolerance || height/2 > tolerance){
        width/=2;
        height/=2;
        DEPTH++;
    }
}

int main(){
    int sizes[] = {100, 200, 400, 500, 600, 1000, 1500, 3000};
    for (int size : sizes) {
        int width = size;
        int height = size;
        int* matrix = new int[width*height]{};

        std::stringstream path;

        path << "maps/A";
        path << size;
        path << ".txt";
        loadMatrix(path.str(), matrix, width, height);
        //loadMatrix("maps/map1-3000", matrix, width, height);
        std::stringstream path2;
        path2 << "izvjestaj2/A";
        path2 << size;
        path2 << ".csv";
        std::ofstream write(path2.str());
        write << "Time[s] :" << std::endl;
        double average{};
        for(int i{}; i<8; i++){
            QuadTree dumir(matrix, width, height);
            Astar astar(&dumir);
            auto start = std::chrono::high_resolution_clock::now();
            astar.FindPath(Point(width/30., height/30.), Point(height*29./30, width/30.));
            astar.FindPath(Point(width/30., height*29./30), Point(height*29./30, width/30.));
            astar.FindPath(Point(width/30., height*29./30), Point(height*29./30, height*29./30));
            astar.FindPath(Point(width/30., height/30.), Point(height*29./30, height*29./30));
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            average += elapsed.count();
            write << elapsed.count() << std::endl;
        }
        write << average/10;
        delete[] matrix;
    }

    /*
    Astar astar(&dumir);
    astar.FindPath(Point(20, 580), Point(580, 20));
    */
    /*
    QuadTree dumir(matrix, width, height);
    Astar astar(&dumir);
    astar.CreateFullMatlabPlot("instructionsB600.m", Point(20, 580), Point(580, 20));
    */
    return 0;
}

/*
int main(){
    int width = 3000;
    int height = 3000;
    int* matrix = new int[width*height]{};
    loadMatrix("A2.txt", matrix, width, height);
    QuadTree dumir(matrix, width, height);
    Astar astar(&dumir);
    astar.CreateFullMatlabPlot("instructions.m", Point(200, 200), Point(2800, 200));
    delete[] matrix;
    return 0;
}
*/

/*
int main(){
    int width = 600;
    int height = 600;
    int* matrix = new int[width*height]{};
    loadMatrix("A.txt", matrix, width, height);
    QuadTree dumir(matrix, width, height);
    std::vector<Node*> cvorovi(dumir.Astar(dumir.FindNode(Point(1, 550)), dumir.FindNode(Point(550, 1))));
    for(Node* node : cvorovi){
        std::cout << "(" << node -> topLeft.x << " " << node -> topLeft.y << "), (" << node -> botRight.x << " " << node -> botRight.y << ")" << std::endl;
    }
    delete[] matrix;
    return 0;
}
*/
/*
int main(){
    srand(time(NULL));
    int width = 600;
    int height = 600;
    int* matrix = new int[width*height]{};
    loadMatrix("A.txt", matrix, width, height);
    QuadTree dumir(matrix, width, height);

        /*
    int width = 64;
    int height = 64;
    QuadTree dumir(Point(0,0), Point(width, height));
    dumir.InsertPoint(Point(3,3));
    dumir.InsertPoint(Point(20,30));
    dumir.InsertPoint(Point(25,25));
    dumir.InsertPoint(Point(40,40));
    */
    //dumir.WriteAll();
    /*
    std::vector<Node*> neighbors(dumir.FindAdjacentNoOccupied(dumir.FindNode(Point(3,3))));
    for(Node* node : neighbors){
        std::cout << "(" << node -> topLeft.x << " " << node -> topLeft.y << "), (" << node -> botRight.x << " " << node -> botRight.y << ")" << std::endl;
    }
    */
    /*
    std::stringstream lokacija;
    lokacija << "reports3/Izvjestaj50x1000";
    lokacija << ".csv";
    std::ofstream izvjestaj(lokacija.str());
    izvjestaj << "time" << std::endl;
    int timer = 0;
    for(int i{}; i<50; i++){
        clock_t vrijeme1 = clock();
        for(int j{}; j<1000; j++){
            int x = SlucajniBroj(0, width-1);
            int y = SlucajniBroj(0, height-1);
            Node* node{dumir.FindNode(Point(x,y))};
            std::vector<Node*> neighbors{dumir.FindAdjacent(node)};
            //timer += (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
            /*
            std::stringstream lokacija2;
            lokacija2 << "reports4/Izvjestaj";
            lokacija2 << i;
            lokacija2 << "x";
            lokacija2 << j;
            lokacija2 << ".csv";
            std::ofstream izvjestaj2(lokacija2.str());

            lokacija2 << "Top Left, Bot Right" << std::endl;
                izvjestaj2 << "(" << node -> topLeft.x << " " << node -> topLeft.y << "), (" << node -> botRight.x << " " << node -> botRight.y << ")" << std::endl;

            for(Node* help : neighbors){
                izvjestaj2 << "(" << help -> topLeft.x << " " << help -> topLeft.y << "), (" << help -> botRight.x << " " << help -> botRight.y << ")" << std::endl;
            }

        }
        clock_t vrijeme2 = clock();
        izvjestaj << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << std::endl;
        timer += (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        //izvjestaj << timer << std::endl;
    }
    izvjestaj << timer << std::endl;
    delete[] matrix;
    return 0;
}
*/

/*
int main(){
    srand(time(NULL));
    int width = 100;
    int height = 100;
    for(int j = 0; j<6; j++){
        std::stringstream lokacija;
        lokacija << "reports/Izvjestaj";
        lokacija << width;
        lokacija << "x";
        lokacija << height;
        lokacija << ".txt";
        std::ofstream izvjestaj(lokacija.str());
        int timer = 0;
        int noder = 0;
        for(int i = 0; i<50; i++){
            int* matrix = new int[width*height]{};
            generateMatrix(matrix, width, height);
            clock_t vrijeme1 = clock();
            QuadTree dumir(matrix, width, height);
            clock_t vrijeme2 = clock();
            int nodes = dumir.countNodes();
            izvjestaj << j << ". "<< width << " " << height << " " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms. " << nodes << " nodes" << std::endl;
            timer += (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
            noder += nodes;
            delete[] matrix;
        }
        izvjestaj << timer << std::endl;
        izvjestaj << noder;
        height *= 2;
        width *= 2;
    }

    width = 100;
    height = 100;
    for(int j = 0; j<10; j++){
        std::stringstream lokacija;
        lokacija << "reports/Izvjestaj2";
        lokacija << width;
        lokacija << "x";
        lokacija << height;
        lokacija << ".txt";
        std::ofstream izvjestaj(lokacija.str());
        int timer = 0;
        int noder = 0;
        for(int i = 0; i<50; i++){
            int* matrix = new int[width*height]{};
            generateMatrix(matrix, width, height);
            clock_t vrijeme1 = clock();
            QuadTree dumir(matrix, width, height);
            clock_t vrijeme2 = clock();
            int nodes = dumir.countNodes();
            izvjestaj << j << ". "<< width << " " << height << " " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms. " << nodes << " nodes" << std::endl;
            timer += (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
            noder += nodes;
            delete[] matrix;
        }
        izvjestaj << timer << std::endl;
        izvjestaj << noder;
        width *= 2;
    }
    return 0;
}
*/
/*
int main() //main za generisanje izvještaja i poreðenje matrica
{
    srand(time(NULL));
    std::ofstream izvjestaj("izvjestajehlimanayes.txt");
    int width = 6400;
    int height = 100;
    for(int i{}; i<3; i++){
        //setDepth(width, height, 1);
        for(int j{}; j<5; j++){
            std::cout << i << " " << j << " ";
            int* matrix = new int[width*height]{};
            //generateMatrix(matrix, width, height);
            generateFullMatrix(matrix, width, height);
            clock_t vrijeme1 = clock();
            QuadTree dumir(matrix, width, height);
            clock_t vrijeme2 = clock();
            izvjestaj << j << ". "<< width << " " << height << " " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms. " << dumir.countNodes() << " nodes" << std::endl;

            std::stringstream lokacija;
            lokacija << "Matrica";
            lokacija << width;
            lokacija << "x";
            lokacija << height;
            lokacija << "x";
            lokacija << j;
            lokacija << ".txt";
            generateTxt(lokacija.str(), matrix, width, height);

            std::stringstream lokacija2;
            lokacija2 << "QuadTree";
            lokacija2 << width;
            lokacija2 << "x";
            lokacija2 << height;
            lokacija2 << "x";
            lokacija2 << j;
            lokacija2 << ".txt";
            std::ofstream ispis(lokacija2.str());

            for(int i = 0; i < width; i++){
                for(int j = 0; j<height; j++){
                    ispis << dumir.isOccupied(Point(i,j));
                }
                ispis << std::endl;
            }

            std::stringstream lokacija3;
            lokacija3 << "DepthQuadTree";
            lokacija3 << width;
            lokacija3 << "x";
            lokacija3 << height;
            lokacija3 << "x";
            lokacija3 << j;
            lokacija3 << ".txt";
            std::ofstream ispis2(lokacija3.str());
            for(int i = 0; i < width; i++){
                for(int j = 0; j<height; j++){
                    ispis2 << dumir.getDepth(Point(i,j));
                }
                ispis2 << std::endl;
            }
            delete[] matrix;
            std::cout << compareFiles(lokacija.str(), lokacija2.str()) << std::endl;
        }
        width *= 2;
        //height *= 2;
    }
    return 0;
}
*/
