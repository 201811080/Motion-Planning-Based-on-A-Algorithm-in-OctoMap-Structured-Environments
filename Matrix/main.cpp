#include <iostream>
#include <stdexcept>
#include <random>
#include <time.h>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <chrono>

#include "Astar.h"
#include "AstarMatrix.h"
#include "AstarQuadTree.h"

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
        if(matrix[y*width + x])
            matrix[y*width + x] = 0;
        else
            i--;
    }
}

void generateFullMatrix(int matrix[], int width, int height){
    for(int i{}; i<width*height; i++){
            matrix[i] = 1;
    }
}

void loadMatrix(const std::string &ime, int matrix[], int width, int height){
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
            izlaz << matrix[j * width + i] << ",";
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

int main_(){

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
        std::stringstream path2;
        path2 << "PathFinding/A";
        path2 << size;
        path2 << ".csv";
        std::ofstream write(path2.str());
        write << "Time[s] :" << std::endl;
        double average{};
        for(int i{}; i<10; i++){

            AstarMatrix astar(matrix, width, height);
            std::vector<Point> d;

            auto start = std::chrono::high_resolution_clock::now();
            d=astar.FindPath(Point(width/15., height/15.), Point(height*14./15, width/15.));
            std::cout << d.size();
            astar.FindPath(Point(width/15., height*14./15), Point(height*14./15, width/15.));
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

int main(){

    std::stringstream path2;
    path2 << "PathFinding/report.csv";
    /*
    path2 << map;
    path2 << size;
    path2 << ".csv";
    */
    std::ofstream write(path2.str());
    write << "Map,Width,Height,PointA,PointB,Time1,Time2,Time3,Time4,Time5,Time6,Time7,Time8,Time9,Time10,AvgTime,Distance" << std::endl;

    int sizes[] = {100, 200, 400, 500, 600, 1000, 1500, 3000};
    char map;
            for (int size : sizes) {
                map = 'A';
                for (int j{}; j < 2; j++) {
                int width = size;
                int height = size;
                int *matrix = new int[width * height]{};

                std::stringstream path;
                path << "maps/";
                path << map;
                path << size;
                path << ".txt";
                loadMatrix(path.str(), matrix, width, height);

                Point TL(width / 15., height / 15.);
                Point BL(width / 15., height * 14. / 15);
                Point TR(width * 14. / 15, height / 15.);
                Point BR(width * 14. / 15, height * 14. / 15);

                AstarMatrix astar(matrix, width, height);

                std::vector<Point> pathpoints;

                //TL TR
                write << map << "," << width << "," << height << "," << "(" << TL.x << " " << TL.y
                      << "),(" << TR.x << " " << TR.y << "),";
                double average{};

                for (int i{}; i < 10; i++) {
                    auto start = std::chrono::high_resolution_clock::now();
                    pathpoints = astar.FindPath(TL, TR);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = end - start;
                    average += elapsed.count();
                    write << elapsed.count() << ",";
                }
                write << average / 10 << ",";
                write << astar.FindDistance(pathpoints, TL, TR) << std::endl;
                std::stringstream test1;
                test1 << "test/D";
                test1 << "M";
                test1 << map << width << "x" << height;
                test1 << "TLTR.m";
                astar.CreateFullMatlabPlot(test1.str(), TL, TR);
                //BL TR
                write << map << "," << width << "," << height << "," << "(" << BL.x << " " << BL.y
                      << "),(" << TR.x << " " << TR.y << "),";
                average = 0;
                for (int i{}; i < 10; i++) {
                    auto start = std::chrono::high_resolution_clock::now();
                    pathpoints = astar.FindPath(BL, TR);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = end - start;
                    average += elapsed.count();
                    write << elapsed.count() << ",";
                }
                write << average / 10 << ",";
                write << astar.FindDistance(pathpoints, BL, TR) << std::endl;
                std::stringstream test2;
                test2 << "test/D";
                test2 << "M";
                test2 << map << width << "x" << height;
                test2 << "BLTR.m";
                astar.CreateFullMatlabPlot(test2.str(), BL, TR);

                //BL BR
                write << map << "," << width << "," << height << "," << "(" << BL.x << " " << BL.y
                      << "),(" << BR.x << " " << BR.y << "),";
                average = 0;
                for (int i{}; i < 10; i++) {
                    auto start = std::chrono::high_resolution_clock::now();
                    pathpoints = astar.FindPath(BL, BR);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = end - start;
                    average += elapsed.count();
                    write << elapsed.count() << ",";
                }
                write << average / 10 << ",";
                write << astar.FindDistance(pathpoints, BL, BR) << std::endl;
                std::stringstream test3;
                test3 << "test/D";
                test3 << "M";
                test3 << map << width << "x" << height;
                test3 << "BLBR.m";
                astar.CreateFullMatlabPlot(test3.str(), BL, BR);

                //TL BR
                write << map << "," << width << "," << height << "," << "(" << TL.x << " " << TL.y
                      << "),(" << BR.x << " " << BR.y << "),";
                average = 0;
                for (int i{}; i < 10; i++) {
                    auto start = std::chrono::high_resolution_clock::now();
                    pathpoints = astar.FindPath(TL, BR);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = end - start;
                    average += elapsed.count();
                    write << elapsed.count() << ",";
                }
                write << average / 10 << ",";
                write << astar.FindDistance(pathpoints, TL, BR) << std::endl;
                std::stringstream test4;
                test4 << "test/D";
                test4 << "M";
                test4 << map << width << "x" << height;
                test4 << "TLBR.m";
                astar.CreateFullMatlabPlot(test4.str(), TL, BR);

                delete[] matrix;

                map++;
            }
        }
    return 0;
}

int main2_(){
    int width = 1000;
    int height = 1000;
    int* matrix = new int[width*height]{};
    //loadMatrix("maps/B1500.txt", matrix, width, height);
    generateFullMatrix(matrix, width, height);
    generateMatrix(matrix, width, height);
    AstarMatrix dumir(matrix, width, height);
    auto start = std::chrono::high_resolution_clock::now();
    dumir.FindPath(Point(0, 0), Point(999, 999));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    dumir.CreateFullMatlabPlot("Kemalinstr.m",Point(0, 0), Point(999, 999));
    generateTxt("Kemal.txt", matrix, width, height);
    std::cout << elapsed.count();
    delete[] matrix;
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
    AstarQuadTree astarquadtree(&dumir);

    /*
    clock_t vrijeme1 = clock();
    astar.FindPath(Point(100, 100), Point(2900, 100));
    clock_t vrijeme2 = clock();
    std::cout << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << std::endl;
    */
    /*
    clock_t vrijeme1 = clock();
    astarquadtree.FindPath(Point(100, 100), Point(2900, 100));
    clock_t vrijeme2 = clock();
    std::cout << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << std::endl;
    */
    /*
    astar.CreateFullMatlabPlot("instructions.txt", Point(200, 200), Point(2800, 200));
    astarquadtree.CreateFullMatlabPlot("instructionsquadtree.txt", Point(200, 200), Point(2800, 200));



    /*
    astar.CreateFullMatlabPlot("instructions.txt", Point(200, 200), Point(2800, 200));
    std::cout << "Hi" << std::endl;
    */
    /*
    astarquadtree.CreateFullMatlabPlot("instructionsquadtree.txt", Point(200, 200), Point(2800, 200));
    std::cout << "Hi" << std::endl;
    */
    /*
    AstarMatrix astarmatrix(matrix, width, height);
    astarmatrix.CreateFullMatlabPlot("instructionsmatrix.txt", Point(200, 200), Point(2800, 200));
    */



    /*
    std::vector<Node*> adjecent{dumir.FindAdjacent(dumir.FindNode(Point(100, 100)))};
    std::cout << std::endl << std::endl << std::endl;
    std::ofstream izvjestaj("neighbors.txt");
    for(Node* help : adjecent){
            izvjestaj << "rectangle('Position',[" << help -> WriteMatlab() << "], 'FaceColor',[";
            if(help -> occupied){
                izvjestaj << "0 0 1]);" << std::endl;
            }
            else {
                izvjestaj << "1 1 0]);" << std::endl;
            }
    }
    */
    /*
    std::ofstream izvjestaj("neighbors.txt");
    Node* a{dumir.FindNode(Point(500, 400))};
    Node* b{dumir.FindNode(Point(505, 415))};
    izvjestaj << "rectangle('Position',[" << a -> WriteMatlab() << "], 'FaceColor',[";
    izvjestaj << "0 0 1]);" << std::endl;
    izvjestaj << "rectangle('Position',[" << b -> WriteMatlab() << "], 'FaceColor',[";
    izvjestaj << "0 0 1]);" << std::endl;

    std::cout << std::endl << std::endl << a -> Overlap(b->topLeft, b->botRight);
    */
    /*
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
int main() //main za generisanje izvje�taja i pore�enje matrica
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

