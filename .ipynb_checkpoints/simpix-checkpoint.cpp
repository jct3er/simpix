// simple example of using ROOT libraries in a C++ program with graphics
// and use of TASImage class

#include "TROOT.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TASImage.h"
#include "TApplication.h"
#include "TSystem.h"
#include "assert.h"
#include <iostream>
#include <stdio.h>
#include "math.h"
using namespace std;

const int ntherm = 10000;

double difference(UInt_t pixelA, UInt_t pixelB){
    UInt_t Ar = pixelA & 0x00ff0000;
    UInt_t Ag = pixelA & 0x0000ff00;
    UInt_t Ab = pixelA & 0x000000ff;

    UInt_t Br = pixelB & 0x00ff0000;
    UInt_t Bg = pixelB & 0x0000ff00;
    UInt_t Bb = pixelB & 0x000000ff;

    double dr = Ar-Br;
    double dg = Ag-Bg;
    double db = Ab-Bb;

    return dr*dr+dg*dg+db*db;
};

double E(UInt_t *src, UInt_t *tgt, int num){
    double cost = 0;
    for (int i=0; i<num; i++){
        cost += difference(src[i],tgt[i]);
    };
    return cost;
};

void update(UInt_t *src, UInt_t *tgt, double T, int num){
    int pos = rand() % num;
    UInt_t pixel = src[pos];
    int dist = max(1, (int)(T*.1*num));
    int dx = rand() % (2*dist + 1) - dist;
    int pos2 = (pos+dx);
    pos2 = (pos2 < 0) ? 0 : pos2;
    pos2 = (pos2 >= num) ? num-1 : pos2;
    UInt_t pix2 = src[pos2];
    
    double dE = difference(src[pos2],tgt[pos]) + difference(src[pos],tgt[pos2]) - difference(src[pos],tgt[pos]) - difference(src[pos2],tgt[pos2]);
    if (dE<=0 || drand48() < exp(-dE/T)){
        src[pos] = pix2;
        src[pos2] = pixel;
    };
};

int main(int argc, char **argv){

  // if (argc<3) {
  //   cout << "Usage: simapix_start image1 image2 <output=out.png>" << endl;
  //   return 0; 
  // }
  TString fsrc=argv[1];
  TString ftgt=argv[2];
  TString fout;
  argc>3 ? fout = argv[3] : fout="Monet.png";
  // cout << "Reading images: source= " << fsrc << " target= " << ftgt << endl;
  // cout << "Output= " << fout << endl;

  TApplication theApp("App", &argc, argv);

  // create image objects
  TASImage *tgt1 = new TASImage(fsrc.Data());
  TASImage *tgt2 = new TASImage(ftgt.Data());
  TASImage *out1 = new TASImage(*tgt1); // start with copy of source
  TASImage *out2 = new TASImage(*tgt2); // start with copy of source

  // Test image geometry, exit if they are not the same dimensions
  assert ( tgt1->GetWidth() == tgt2->GetWidth() && tgt1->GetHeight() == tgt2->GetHeight() );
  cout << "Pixel Geometry: " << tgt1->GetWidth() << " x " << tgt1->GetHeight() << endl;
  Long_t numPix=tgt1->GetWidth()*tgt1->GetHeight();

  // *** The work happens here
  // access the pixels for the output image 
  // each pixel is a 32-bit word, 1 byte each for (alpha,red,green,blue)
  // don't touch alpha (bits 31:28)
  UInt_t *imin1 = tgt1->GetArgbArray(); 
  UInt_t *imin2 = tgt2->GetArgbArray(); 
  UInt_t *imout1 = out1->GetArgbArray(); 
  UInt_t *imout2 = out2->GetArgbArray(); 


  int Tmax = 30000;
  int nT = 20000;

  double T;


  for (int itemp=nT; itemp>0; itemp--) {
    T = (Tmax*itemp)/nT;

    if (fmod(T, 10.0)==0) cout << "Temperature: " << T << endl;



    for(int n=0; n<ntherm; n++){
        update(imout1, imin2, T, numPix);
        update(imout2, imin1, T, numPix);    
    };

  };

  

  // *************************


  // print the results
  TCanvas *c1 = new TCanvas("c1", "images", 640, 480);
  c1->Divide(2,2);

  c1->cd(1);
  c1->Draw();
  tgt1->Draw("X");
  c1->cd(2);
  tgt2->Draw("X");
  c1->cd(3);
  out1->Draw("X");
  c1->cd(4);
  out2->Draw("X");
  c1->Print("Monet.png");
  
  // save the new image
  //out->WriteImage(fout.Data());

  // coment out the lines for running in batch mode
  cout << "Press ^c to exit" << endl;
  theApp.SetIdleTimer(30,".q");  // set up a failsafe timer to end the program  
  theApp.Run();

}
