//g++ -std=c++11 main.cpp `pkg-config --libs --cflags opencv4` -o result
//std::string newstr = std::string("{\"photoPaths\":[\"/a/b/c/0.png\",\"/a/b/c/1.png\",\"/a/b/c/2.png\"],\"templateParameters\":{\"finalWidth\":720,\"finalHeight\":1280,\"totalNumberOfPhotos\":3}}");
#include "opencv.hpp"
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace cv;
using namespace std;
using namespace rapidjson;

#define DIR0 "/home/shiyang/document/wooork/ppp/src/"
#define DIR1 "/home/shiyang/Documents/wooork/ppp/src/"

string _strPrefix;
string _strJson;

int _finalWidth = 640;
int _finalHeight = 864;

Mat _photo0;
string _path0 = "./0.jpg";

Mat _photo1;
string _path1 = "./1.jpg";

Mat _photo2;
string _path2 = "/home/document/wooork/ppp/resources/input/2.jpg";

Mat _photoR;

void printOpenCVInfo()
{
    cout << "OpenCV version : " << CV_VERSION << endl;
    cout << "Major version : " << CV_MAJOR_VERSION << endl;
    cout << "Minor version : " << CV_MINOR_VERSION << endl;
    cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;
}

void jsonTest()
{
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse(json);

    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
}

void loadPhotosTest()
{
    Mat img = imread("./test.jpg");
    if (img.empty())
    {
        return;
    }
    namedWindow("Example1", WINDOW_NORMAL);
    imshow("Example1", img); 
    waitKey(0);
    destroyWindow("Example1");
}

int loadConfiguration() 
{
    const char* json = _strJson.c_str();
    Document d;
    d.Parse(json);
    if (d.HasParseError()) {
        cout << "shiyang d has parse error" << endl;
        return -1;
    }

#if 0
    if (d.HasMember("name") && d["name"].IsString()) {
        fprintf(stdout, "name: %s\n", dom["name"].GetString());
    }
    if (dom.HasMember("age") && dom["age"].IsInt()) {
        fprintf(stdout, "age: %d\n", dom["age"].GetInt());
    }

    if (dom.HasMember("value1") && dom["value1"].IsArray()) {
        const rapidjson::Value& arr = dom["value1"];
        for (int i = 0; i < arr.Size(); ++i) {
            const rapidjson::Value& tmp = arr[i];		
            fprintf(stdout, "value1:\ni = %d:", i);
            for (int j = 0; j < tmp.Size(); ++j) {
                if (tmp[j].IsInt())
                    fprintf(stdout, "%d, ", tmp[j].GetInt());
                if (tmp[j].IsFloat())
                    fprintf(stdout, "%.1f, ", tmp[j].GetFloat());
            }
            fprintf(stdout, "\n");
        }
    }
#endif
  
    if (d.HasMember("templateParameters") && d["templateParameters"].IsObject()) {
        const rapidjson::Value& obj = d["templateParameters"];
        if (obj.HasMember("finalWidth") && obj["finalWidth"].IsInt()) {
            //fprintf(stdout, "address: %s\n", utf8_to_gbk(obj["address"].GetString()));
            cout << "shiyang final width = " << obj["finalWidth"].GetInt() << endl;
        }
        if (obj.HasMember("finalHeight") && obj["finalHeight"].IsInt()) {
            //fprintf(stdout, "address: %s\n", utf8_to_gbk(obj["address"].GetString()));
            cout << "shiyang final height = " << obj["finalHeight"].GetInt() << endl;
        }
        if (obj.HasMember("numberOfPhotos") && obj["numberOfPhotos"].IsInt()) {
            //fprintf(stdout, "cat: %d\n", obj["cat"].GetBool());
            cout << "shiyang number of photos = " << obj["numberOfPhotos"].GetInt() << endl;
        }
    } else {
        cout << "shiyang HasMember templateParameters error or templateParameters is object error" << endl;
    }

    if (d.HasMember("photoPaths") && d["photoPaths"].IsArray()) {
        const rapidjson::Value& objArray = d["photoPaths"];
        for (int i = 0; i < objArray.Size(); ++i) {
            //fprintf(stdout, "%.2f, ", arr[i].GetFloat());
            cout << "shiyang photoPaths = " << objArray[i].GetString() << endl;
        }
    } else {
        cout << "shiyang HasMember photoPaths error or photoPaths is array error" << endl;
    }
}

int loadPhotos()
{
    _photo0 = imread(_path0.c_str());
    if (_photo0.empty()) {
        printf("\nshiyang read photo0 failed %s", _path0.c_str());
        return -1;
    }
    _photo1 = imread(_path1.c_str());
    if (_photo1.empty()) {
        printf("\nshiyang read photo1 failed %s", _path1.c_str());
        return -1;
    }
    _photoR = _photo0.clone();
    return 0;
}

inline int clamp(int _v, int _min, int _max) 
{
    if (_min > _max) {
        printf("shiyang clamp error %d > %d", _min, _max);
        return _v;
    }

    if (_v <_min)  {
        _v = _min;
    } else if (_v > _max) {
        _v = _max;
    }
    return _v;
}

inline int mix(int _v0, int _v1, float _p)
{
    return (int)(_v0*(1.0-_p) + _v1*_p);
}

inline int mixColor1i(int _v0, int _v1, float _p)
{
    return clamp((int)(_v0*(1.0-_p) + _v1*_p), 0, 255);
}

void processPhotos()
{
    int w = _photo0.cols;
    int h = _photo0.rows;
    cout << "shiyang w="<<w<<",h="<<h<<endl;
    for (int p=0; p<50; p++) {
        for (int i=0; i<h; i++) {
            for (int j=0; j<w; j++) {
                int r0 = _photo0.at<Vec3b>(i, j)[2];
                int g0 = _photo0.at<Vec3b>(i, j)[1];
                int b0 = _photo0.at<Vec3b>(i, j)[0];
                int r1 = _photo1.at<Vec3b>(i, j)[2];
                int g1 = _photo1.at<Vec3b>(i, j)[1];
                int b1 = _photo1.at<Vec3b>(i, j)[0];
                float progress = p*3.0/49.0;
                //progress = 0.5;
                int rf = mixColor1i(r0, r1, progress);
                int gf = mixColor1i(g0, g1, progress);
                int bf = mixColor1i(b0, b1, progress);
                _photoR.at<Vec3b>(i, j)[2] = rf;
                _photoR.at<Vec3b>(i, j)[1] = gf;
                _photoR.at<Vec3b>(i, j)[0] = bf;
            }
        }
        
        string path = "out";
        path.append(_strPrefix);
        std::stringstream ss;
        if (p<10) {ss << "0";}
        ss << p;
        path.append(ss.str());
        path.append(".jpg");
        imwrite(path, _photoR);      
    }
}

void processVideo()
{
    //system("ls -la");
    //system("ffmpeg -version");
    string strCmd = "ffmpeg -f image2 -i out";
    strCmd.append(_strPrefix);
    strCmd.append("%2d.jpg -vcodec libx264 -r 30 -pix_fmt yuv420p out");
    strCmd.append(_strPrefix);
    strCmd.append(".mp4 -y");
    //system("ffmpeg -f image2 -i out%2d.jpg -vcodec libx264 -r 30 -pix_fmt yuv420p out.mp4 -y");
    //cout << strCmd << endl;
    system(strCmd.c_str());
}

int _n = 0;
void calTest(float val) {
    cout << "cal test val = " << val << endl;
    if (val > 0.0f) {
        _n++;
        calTest((val-3.0f)*1.023f);  
    } else {
        cout << "cal test end n = " << _n << endl;
    }
}

void showResultsTest()
{
    namedWindow("show", WINDOW_NORMAL);
    imshow("show", _photoR); 
    waitKey(0);
    destroyWindow("show");
}

int main( int argc, char** argv )
{
    cout << "" << endl;
    cout << "shiyang main" << endl;
    
    cout << "shiyang argc " << argc << endl;
    for (int i=0; i<argc; i++) {
        cout << "shiyang argv " << argv[i] << endl;
    }
    if (argc>1) {
        _strPrefix = argv[1];
    }
    cout << "shiyang prefix " << _strPrefix << endl;
    if (argc>2) {
        _strJson = argv[2];
    }
    cout << "shiyang json " << _strJson << endl;

    printOpenCVInfo();
    // parse argc&argv

    // json
    jsonTest();

    loadConfiguration();
   
    //loadPhotosTest();
    loadPhotos();

    processPhotos();

    processVideo();
    
    calTest(60.0f);
    
    //showResultsTest();
    //generateResults();

    cout << "" << endl;

    return 0;
}  
