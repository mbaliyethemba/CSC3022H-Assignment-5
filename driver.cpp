#include <iostream>
#include "audio.h"

int getIntFromArgs(char * arg);

float getFloatFromArgs(char * arg);

using namespace std;
using namespace SHNMBA004;

int main(int argc, char * argv[]){
	//checks if right amount of arguments
	if (argc > 16 || argc < 9){
		cout << "Incorrect arguments passed" << endl;
	}
	else{
		int sampleRate = getIntFromArgs(argv[2]);
		bool bits = (string(argv[4]) == "8bit");
		int channel = getIntFromArgs(argv[6]);
		string outfile = "out";
		int index = 7;
		if(string(argv[7]) == "-o"){
			outfile = argv[8];
			index = 9;
		}
		string op = argv[index];
		if(op == "-add"){
			index++;
			string infile = argv[index];
			index++;
			string infile1 = argv[index];
			if(bits && (channel == 1)){
				//8bit and mono
				audio<int8_t> audIn = audio<int8_t>(infile,channel,sampleRate);
				audio<int8_t> audIn1 = audio<int8_t>(infile1,channel,sampleRate);
				audio<int8_t> addAud = audIn + audIn1;
				addAud.saveAudio(outfile);
			}
			else if(!bits && (channel == 1)){
				//16bit and mono
				audio<int16_t> audIn = audio<int16_t>(infile,channel,sampleRate);
				audio<int16_t> audIn1 = audio<int16_t>(infile1,channel,sampleRate);
				audio<int16_t> addAud = audIn + audIn1;
				addAud.saveAudio(outfile);
			}
			else if(bits && (channel == 2)){
				//8bit and stereo
				audio<pair<int8_t,int8_t>> audIn = audio<pair<int8_t,int8_t>>(infile,channel,sampleRate);
				audio<pair<int8_t,int8_t>> audIn1 = audio<pair<int8_t,int8_t>>(infile1,channel,sampleRate);
				audio<pair<int8_t,int8_t>> addAud = audIn + audIn1;
				addAud.saveAudio(outfile);
			}
			else if(!bits && (channel == 2)){
				//16bit and stereo
				audio<pair<int16_t,int16_t>> audIn = audio<pair<int16_t,int16_t>>(infile,channel,sampleRate);
				audio<pair<int16_t,int16_t>> audIn1 = audio<pair<int16_t,int16_t>>(infile1,channel,sampleRate);
				audio<pair<int16_t,int16_t>> addAud = audIn + audIn1;
				addAud.saveAudio(outfile);
			}
		}
		else if (op == "-cut"){
			index++;
			int r1 = getIntFromArgs(argv[index]);
			int r2 = getIntFromArgs(argv[index]);
			pair<int,int> cutRange = make_pair(r1,r2);
			string infile = argv[index];
			if (bits && (channel==1)){ 
				//8bit and Mono
                audio<int8_t> audIn = audio<int8_t>(infile,channel,sampleRate);
                audio<int8_t> cutAudio = audIn^cutRange;
                cutAudio.saveAudio(outfile);
            }
            else if (!bits && (channel == 1)){ 
				//16bit and Mono
                audio<int16_t> audIn = audio<int16_t>(infile,channel,sampleRate);
                audio<int16_t> cutAudio = audIn^cutRange;
                cutAudio.saveAudio(outfile);
            }
            else if(bits && (channel==2)){ 
				//8bit and Stereo
                audio<pair<int8_t,int8_t>> audIn = audio<pair<int8_t,int8_t>>(infile,channel,sampleRate);
                audio<pair<int8_t,int8_t>> cutAudio = audIn^cutRange;
                cutAudio.saveAudio(outfile);
            }
            else if(!bits && (channel==2)){ 
				//16bit and Stereo
                audio<pair<int16_t,int16_t>> audIn = audio<pair<int16_t,int16_t>>(infile,channel,sampleRate);
                audio<pair<int16_t,int16_t>> cutAudio = audIn^cutRange;
                cutAudio.saveAudio(outfile);
            }
		}
		
		else if (op == "-radd"){
            int r1  = getIntFromArgs(argv[index]);
            index++;
            int r2 = getIntFromArgs(argv[index]);
            pair<int,int> cutRange = make_pair(r1,r2);
            index++;
            string infile = argv[index];
            index++;
            string infile1 = argv[index];
            if (bits && (channel==1)){ 
				//8bit and Mono
                audio<int8_t> audIn = audio<int8_t>(infile,channel,sampleRate);
                audio<int8_t> audIn1 = audio<int8_t>(infile1,channel,sampleRate);
                audio<int8_t> addAudio = audIn.rangeAdd(audIn1,cutRange);
                addAudio.saveAudio(outfile);
            }
            else if (!bits && (channel == 1)){ 
				//16bit and Mono
                audio<int16_t> audIn = audio<int16_t>(infile,channel,sampleRate);
                audio<int16_t> audIn1 = audio<int16_t>(infile1,channel,sampleRate);
                audio<int16_t> addAudio = audIn.rangeAdd(audIn1,cutRange);
                addAudio.saveAudio(outfile);
            }
            else if(bits && (channel==2)){ 
				//8bit and Stereo
                audio<pair<int8_t,int8_t>> audIn = audio<pair<int8_t,int8_t>>(infile,channel,sampleRate);
                audio<pair<int8_t,int8_t>> audIn1 = audio<pair<int8_t,int8_t>>(infile1,channel,sampleRate);
                audio<pair<int8_t,int8_t>> addAudio = audIn.rangeAdd(audIn1,cutRange);
                addAudio.saveAudio(outfile);
            }
            else if(!bits && (channel==2)){ 
				//16bit and Stereo
                audio<pair<int16_t,int16_t>> audIn = audio<pair<int16_t,int16_t>>(infile,channel,sampleRate);
                audio<pair<int16_t,int16_t>> audIn1 = audio<pair<int16_t,int16_t>>(infile1,channel,sampleRate);
                audio<pair<int16_t,int16_t>> addAudio = audIn.rangeAdd(audIn1,cutRange);
                addAudio.saveAudio(outfile);
            }
	}
	else if (op == "-cat"){
            index++;
            string infile = argv[index];
            index++;
            string infile1 = argv[index];
            if (bits && (channel==1)){ 
				//8bit and Mono
                audio<int8_t> audIn = audio<int8_t>(infile,channel,sampleRate);
                audio<int8_t> audIn1 = audio<int8_t>(infile1,channel,sampleRate);
                audio<int8_t> addAudio = audIn | audIn1;
                addAudio.saveAudio(outfile);
            }
            else if (!bits && (channel == 1)){ 
				//16bit and Mono
                audio<int16_t> audIn = audio<int16_t>(infile,channel,sampleRate);
                audio<int16_t> audIn1 = audio<int16_t>(infile1,channel,sampleRate);
                audio<int16_t> addAudio = audIn | audIn1;
                addAudio.saveAudio(outfile);
            }
            else if(bits && (channel==2)){ 
				//8bit and Stereo
                audio<pair<int8_t,int8_t>> audIn = audio<pair<int8_t,int8_t>>(infile,channel,sampleRate);
                audio<pair<int8_t,int8_t>> audIn1 = audio<pair<int8_t,int8_t>>(infile1,channel,sampleRate);
                audio<pair<int8_t,int8_t>> addAudio = audIn | audIn1;
                addAudio.saveAudio(outfile);
            }
            else if(!bits && (channel==2)){ 
				//16bit and Stereo
                audio<pair<int16_t,int16_t>> audIn = audio<pair<int16_t,int16_t>>(infile,channel,sampleRate);
                audio<pair<int16_t,int16_t>> audIn1 = audio<pair<int16_t,int16_t>>(infile1,channel,sampleRate);
                audio<pair<int16_t,int16_t>> addAudio = audIn | audIn1;
                addAudio.saveAudio(outfile);
            }
		}
		
		 else if (op == "-v"){
            index++; //to get to first volumefactor
            float v1  = getFloatFromArgs(argv[index]);

            pair<int,int> volumeFactorPair;
            if (channel ==2){
                index++;
                float v2 = getFloatFromArgs(argv[index]);
                volumeFactorPair = make_pair(v1,v2);
            }

            index++;
            string infile = argv[index];

            if (bits && (channel==1)){ 
				//8bit and Mono
                audio<int8_t> audIn = audio<int8_t>(infile,channel,sampleRate);
                audio<int8_t> cutAudio = audIn * v1;
                cutAudio.saveAudio(outfile);
            }
            else if (!bits && (channel == 1)){ 
				//16bit and Mono
                audio<int16_t> audIn = audio<int16_t>(infile,channel,sampleRate);
                audio<int16_t> cutAudio = audIn * v1;
                cutAudio.saveAudio(outfile);
            }
            else if(bits && (channel==2)){ 
				//8bit and Stereo
                audio<pair<int8_t,int8_t>> audIn = audio<pair<int8_t,int8_t>>(infile,channel,sampleRate);
                audio<pair<int8_t,int8_t>> cutAudio = audIn * volumeFactorPair;
                cutAudio.saveAudio(outfile);
            }
            else if(!bits && (channel==2)){
				 //16bit and Stereo
                audio<pair<int16_t,int16_t>> audIn = audio<pair<int16_t,int16_t>>(infile,channel,sampleRate);
                audio<pair<int16_t,int16_t>> cutAudio = audIn * volumeFactorPair;
                cutAudio.saveAudio(outfile);
            }
		}
		
		else if (op == "-rms"){
            index++;
            string infile = argv[index];
            if (bits && (channel==1)){ 
				//8bit and Mono
                audio<int8_t> audIn = audio<int8_t>(infile,channel,sampleRate);
                float rms = audIn.RMS();
                cout<<"RMS is: "<< rms<<endl;
            }
            else if (!bits && (channel == 1)){ 
				//16bit and Mono
                audio<int16_t> audIn = audio<int16_t>(infile,channel,sampleRate);
                cout<<"RMS is: "<<audIn.RMS()<<endl;
            }
            else if(bits && (channel==2)){ 
				//8bit and Stereo
                audio<pair<int8_t,int8_t>> audIn = audio<pair<int8_t,int8_t>>(infile,channel,sampleRate);
                cout<<"RMS for Left side is: "<<audIn.RMS().first<<endl;
                cout<<"RMS for Right side is: "<<audIn.RMS().second<<endl;
            }
            else if(!bits && (channel==2)){ 
				//16bit and Stereo
                audio<pair<int16_t,int16_t>> audIn = audio<pair<int16_t,int16_t>>(infile,channel,sampleRate);
                cout<<"RMS for Left side is: "<<audIn.RMS().first<<endl;
                cout<<"RMS for Right side is: "<<audIn.RMS().second<<endl;
            }	
		}
		
		else if (op == "-rev"){
            index++;
            string infile = argv[index];
            if (bits && (channel==1)){ 
				//8bit and Mono
                audio<int8_t> audIn = audio<int8_t>(infile,channel,sampleRate);
                audIn.reverse();
                audIn.saveAudio(outfile);
            }
            else if (!bits && (channel == 1)){ 
				//16bit and Mono
                audio<int16_t> audIn = audio<int16_t>(infile,channel,sampleRate);
                audIn.reverse();
                audIn.saveAudio(outfile);
            }
            else if(bits && (channel==2)){ 
				//8bit and Stereo
                audio<pair<int8_t,int8_t>> audIn = audio<pair<int8_t,int8_t>>(infile,channel,sampleRate);
                audIn.reverse();
                audIn.saveAudio(outfile);
            }
            else if(!bits && (channel==2)){ 
				//16bit and Stereo
                audio<pair<int16_t,int16_t>> audIn = audio<pair<int16_t,int16_t>>(infile,channel,sampleRate);
                audIn.reverse();
                audIn.saveAudio(outfile);
            }
		}
		
		else if (op == "-norm"){
            index++;
            float r1  = getFloatFromArgs(argv[index]);

            pair<int,int> normPair ;
            if (channel ==2){
                index++;
                float r2 = getFloatFromArgs(argv[index]);
                normPair = make_pair(r1,r2);
            }

            index++;
            string infile = argv[index];

            if (bits && (channel==1)){ 
				//8bit and Mono
                audio<int8_t> audIn = audio<int8_t>(infile,channel,sampleRate);
                audio<int8_t> cutAudio = audIn.normalize(r1);
                cutAudio.saveAudio(outfile);
            }
            else if (!bits && (channel == 1)){ 
				//16bit and Mono
                audio<int16_t> audIn = audio<int16_t>(infile,channel,sampleRate);
                audio<int16_t> cutAudio = audIn.normalize(r1);
                cutAudio.saveAudio(outfile);
            }
            else if(bits && (channel==2)){ 
				//8bit and Stereo
                audio<pair<int8_t,int8_t>> audIn = audio<pair<int8_t,int8_t>>(infile,channel,sampleRate);
                audio<pair<int8_t,int8_t>> cutAudio = audIn.normalize(normPair);
                cutAudio.saveAudio(outfile);
            }
            else if(!bits && (channel==2)){ 
				//16bit and Stereo
                audio<pair<int16_t,int16_t>> audIn = audio<pair<int16_t,int16_t>>(infile,channel,sampleRate);
                audio<pair<int16_t,int16_t>> cutAudio = audIn.normalize(normPair);
                cutAudio.saveAudio(outfile);
            }
		}
}
return 0;
}
int getIntFromArgs(char * arg){
    stringstream argString(arg);
    int threshold;
    argString >>threshold;
    return threshold;
}
float getFloatFromArgs(char * arg){
    stringstream argString(arg);
    float threshold;
    argString >>threshold;
    return threshold;
}

