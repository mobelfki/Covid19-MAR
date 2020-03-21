#include "TTree.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TVirtualFitter.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TCanvas.h"


void plot(){
    TTree *T = new TTree("ntuple","data from csv file");
    Long64_t nlines = T->ReadFile("coronaMar2.csv");

    Float_t d,x1,x2,errx2;

    T->SetBranchAddress("Days", &d);
    T->SetBranchAddress("Cases", &x1);
    T->SetBranchAddress("New Cases", &x2);
    T->SetBranchAddress("Sum Error", &errx2);

	
	int n = T->GetEntriesFast();
	
	Float_t x[19], y[19], erry[19], errx[19];
	for(int i = 0; i<n; i++)
	{
		T->GetEntry(i);
		x[i] = d;
		y[i] = x1;
		erry[i] = errx2;
		errx[i] = 0;
	}

		TGraph* gr = new TGraph(n,x,y);
		gr->SetMarkerStyle(21);
		gr->SetMarkerSize(1.);
		gr->SetMarkerColor(4);
		gr->SetTitle("Covid-19 infections in Morocco");
		gr->GetXaxis()->SetTitle("Days in March 2020");
		gr->GetYaxis()->SetTitle("Cumulative Covid-19 cases");

		TF1 *f_l = new TF1("logi", "[4]/([2]+[0]*exp([1]*x+[3]))", 0, 30);
		gr->Fit(f_l,"RMEQ");

		TGraphErrors* grint95 = new TGraphErrors(31);
		TGraphErrors* grint68 = new TGraphErrors(31);
		for(int i = 0; i<n; i++)
		{
			grint95->SetPoint(i, gr->GetX()[i], gr->GetY()[i]);
			grint68->SetPoint(i, gr->GetX()[i], gr->GetY()[i]);
		}
		for(int i = n; i<31; i++)
		{
			grint95->SetPoint(i, i, f_l->Eval(i+.5));
			grint68->SetPoint(i, i, f_l->Eval(i+.5));
		}	
		(TVirtualFitter::GetFitter())->GetConfidenceIntervals(grint95, .9999);
		(TVirtualFitter::GetFitter())->GetConfidenceIntervals(grint68, .95);

		grint95->SetLineColor(kRed);
		grint95->SetFillColorAlpha(5, 0.5);

		grint95->SetTitle("Covid-19 infections in Morocco");
		grint95->GetXaxis()->SetTitle("Days in March 2020");
		grint95->GetYaxis()->SetTitle("Cumulative Covid-19 cases");

		grint95->GetXaxis()->SetRangeUser(0,30);
		grint95->GetYaxis()->SetRangeUser(0,100);

		TCanvas* c = new TCanvas("Covid19-MAR","Covid19-MAR",750,750);

		grint95->Draw("ape3");

		grint68->SetLineColor(kRed);
		grint68->SetFillColorAlpha(3, 1.);
		grint68->Draw("e3same");
		
		gr->Draw("psame");

		auto legend = new TLegend(0.1,0.7,0.48,0.9);
		legend->AddEntry(gr,"Data");
		legend->AddEntry(grint95, "99.99% Conf. Int. (#pm 5#sigma)");
		legend->AddEntry(grint68, "95% Conf. Int. (#pm 2#sigma)");
		legend->AddEntry(f_l, Form("Fit, #chi2/ndf = %.1f",f_l->GetChisquare()/f_l->GetNDF()));
		legend->Draw("same");

		auto X    = grint95->GetX();
		auto Y    = grint95->GetY();
		auto EY95 = grint95->GetEY();
		auto EY68 = grint68->GetEY();

		auto t0 = new TLatex(1,60,"  Day     : Cases #pm 2#sigma #pm 5#sigma");
		auto t1 = new TLatex(1,55,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[19], Y[19] , EY68[19] , EY95[19]));
		auto t2 = new TLatex(1,50,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[20], Y[20] , EY68[20] , EY95[20]));
		auto t3 = new TLatex(1,45,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[21], Y[21] , EY68[21] , EY95[21]));
		auto t4 = new TLatex(1,40,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[22], Y[22] , EY68[22] , EY95[22]));
		auto t5 = new TLatex(1,35,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[23], Y[23] , EY68[23] , EY95[23]));
		auto t6 = new TLatex(1,30,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[30], Y[30] , EY68[30] , EY95[30]));
		t0->SetTextSize(0.035);
		t1->SetTextSize(0.035);
		t2->SetTextSize(0.035);
		t3->SetTextSize(0.035);
		t4->SetTextSize(0.035);
		t5->SetTextSize(0.035);
		t6->SetTextSize(0.035);
		t0->Draw("same");
		t1->Draw("same");
		t2->Draw("same");
		t3->Draw("same");
		t4->Draw("same");
		t5->Draw("same");
		t6->Draw("same");

		c->SaveAs("result.png");
}
