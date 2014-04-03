//Create and Draw a TMultiGraph
//Author:: Rene Brun

#include<TGraphErrors.h>

void multigraph(){
   gStyle->SetOptFit();
   TCanvas *c1 = new TCanvas("c1","multigraph",700,500);
   c1->SetGrid();

      // draw a frame to define the range
   TMultiGraph *mg = new TMultiGraph();

      // create first graph
   const Int_t n1 = 10;
   Double_t x1[]  = {1.02421,
     1.71628,
     2.65643,
     3.63391,
     4.61575,
     5.60596,
     6.59172,
     7.60532,
     8.58666,
     9.58648};
   Double_t y1[]  = {0.0289587,
     0.0238765,
     0.0215314,
     0.0199691,
     0.0184914,
     0.0182808,
     0.017711,
     0.0172155,
     0.0170043,
     0.01603
   };

   Double_t ex1[] = {0.087554,
     0.114345,
       0.205181,
       0.262236,
       0.352799,
       0.414018,
       0.517634,
       0.427648,
       0.589271,
       0.610282};
   Double_t ey1[] = {
0.000250397,
   0.000200788,
   0.000190442,
   0.000179945,
   0.000143735,
   0.00015511,
  0.000155632,
   0.000156257,
   0.000153043,
  0.000148542};



   TGraphErrors *gr1 = new TGraphErrors(n1,x1,y1,ex1,ey1);
   gr1->SetMarkerColor(kBlue);
   gr1->SetMarkerStyle(21);
   TF1 *fa = new TF1("fa","sqrt(pow([0],2)+pow([1],2)/x+pow([2],2)/x/x)",0,10);
   //TF1 *fa = new TF1("fa","sqrt([0]*[0]+[1]*[1]/x+[2]*[2]/x/x)",0,10);
   //TF1 *fa = new TF1("fa","[0]/sqrt(x)+[1]",0,10);
   fa->SetLineColor(kBlue);
   gr1->Fit("fa","q");
   mg->Add(gr1);

      // create second graph
   const Int_t n2 = 10;


Double_t x2[]  = {
1.02421,
1.71628,
2.65643,
3.63391,
4.61575,

5.60596,
6.59172,
7.60532,
8.58666,
9.58648};

   Double_t y2[]  = {
0.0393076,
0.031519, 
0.0271052,
0.0247132,
0.0235158,

0.0219442,
0.0210609,
0.0202897,
0.0199039,
0.018683 };


 Double_t ex2[] = {
   0.087554,
0.114345,
0.205181,
0.262236,
0.352799,

0.414018,
0.517634,
0.427648,
0.589271,
0.610282};



   Double_t ey2[] = {
 0.000334332,
 0.000266838,
 0.000217308,
 0.000205775,
 0.000205418,

 0.000181234,
 0.000188549,
 0.000182179,
 0.000182594,
0.000155563
   };

   TGraphErrors * gr2 = new TGraphErrors(n2,x2,y2,ex2,ey2);
   gr2->SetMarkerColor(kRed);
   gr2->SetMarkerStyle(20);
   //TF1 *fb = new TF1("fb","[0]/sqrt(x)+[1]",0,10);
   TF1 *fb = new TF1("fb","sqrt([0]+[1]/x+[2]/x/x)",0,10);
   TF1 *fc = new TF1("fc","sqrt(pow([0],2)+pow([1],2)/x+pow([2],2)/x/x)",0,10);
   //TF1 *fb = new TF1("fb","sqrt([0]*[0]+[1]*[1]/x+[2]*[2]/x/x)",0,10);
  fb->SetLineColor(kRed);
   gr2->Fit("fb","q");
  // fc->FixParameter(0,sqrt(fb->GetParameter(0)));
  // fc->FixParameter(1,sqrt(fb->GetParameter(1)));
  // fc->FixParameter(2,sqrt(-1*fb->GetParameter(2)));
  //gr2->Fit("fc","q"); 
   mg->Add(gr2);
   
   mg->Draw("ap");
   
     //force drawing of canvas to generate the fit TPaveStats
   c1->Update();
   TPaveStats *stats1 = (TPaveStats*)gr1->GetListOfFunctions()->FindObject("stats");
   TPaveStats *stats2 = (TPaveStats*)gr2->GetListOfFunctions()->FindObject("stats");
   stats1->SetTextColor(kBlue); 
   stats2->SetTextColor(kRed); 
   stats1->SetX1NDC(0.12); stats1->SetX2NDC(0.32); stats1->SetY1NDC(0.75);
  stats2->SetX1NDC(0.72); stats2->SetX2NDC(0.92); stats2->SetY1NDC(0.78);
   c1->Modified();
   return c1;
}
