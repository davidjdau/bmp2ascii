
//standard library includes
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

//import namespace std
using namespace std;

int main(int argc, char* argv[]){

  if (argc != 3){
    cout << "usage: " << argv[0] << " filename.bmp" << " filename.txt" << endl;
    return 1;
  }

  ifstream file(argv[1],ios::binary);

  if(file){
    file.seekg(0,ios::end);
    streampos length = file.tellg();
    file.seekg(0,ios::beg);

    vector<char> buffer;
    buffer.resize(length);
    file.read(&buffer[0],length);

    int width =  *(int*)&buffer[18];
    int height =  *(int*)&buffer[22];

    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    FILE* f = fopen(argv[1],"rb");
    fseek(f,54,SEEK_SET);
    ofstream ofs (argv[2], ofstream::out);
    for(int i = 0; i < height; i++){
      fread (data, sizeof(unsigned char), row_padded,f);
      for(int j = 0; j < width * 3; j += 3){
        tmp = data[j];
        data[j] = data[j+2];
        data[j+2] = tmp;

        int r, g, b;
        r = (int)data[j];
        g = (int)data[j+1];
        b = (int)data[j+2];

        int cSum = r + b + g;

        if(cSum == 0){
          cout << '#';
          ofs << '#';
        }else if(cSum == 765){
          cout << ' ';
          ofs << ' ';
        }else{
          if ((g + b) < (r + b)){//magenta *
            cout << '*';
            ofs << '*';
          }else if ((r + g) < (b + g)){//cyan %
            cout << '%';
            ofs << '%';
          }
          else if ((b + g) < (r + g)){//yellow ^
            cout << '^';
            ofs << '^';
          }else if(r > (b + g)){//red !
            cout << '!';
            ofs << '!';
          }
          else if (b > (r + g)){//blue @
            cout << '@';
            ofs << '@';
          }
          else if (g > (r + b)){//green $
            cout << '$';
            ofs << '$';
          }else{ //any other garbage &
            cout << '&';
            ofs << '&';
          }
        }
      }
      cout << endl;
      ofs << '\n';
    }
    ofs.close();
    delete [] data;
  }


  return 0;
}
