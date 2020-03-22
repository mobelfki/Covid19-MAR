#include "TTree.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TVirtualFitter.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TCanvas.h"


void plot(){
    TTree *T = new TTree("ntuple","data from csv file");
    Long64_t nlines = T->ReadFile("coronaMar.csv");

    Float_t d,x1,x2,errx2;

    T->SetBranchAddress("Days", &d);
    T->SetBranchAddress("Cases", &x1);
    T->SetBranchAddress("New Cases", &x2);
	
	const int n = T->GetEntriesFast()-8;
	
	Float_t x[n], y[n], erry[n], errx[n];
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

		TF1 *f_l = new TF1("logi", "[0]*exp([1]*x+[2]) + [3]*x", 2, 31);
	
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
			
			grint95->SetPoint(i, i+1, f_l->Eval(i+1));
			grint68->SetPoint(i, i+1, f_l->Eval(i+1));
		}	
		(TVirtualFitter::GetFitter())->GetConfidenceIntervals(grint95, .9999);
		(TVirtualFitter::GetFitter())->GetConfidenceIntervals(grint68, .95);

		grint95->SetLineColor(kRed);
		grint95->SetFillColorAlpha(5, 0.5);

		grint95->SetTitle("Covid-19 infections in Morocco");
		grint95->GetXaxis()->SetTitle("Days in March 2020");
		grint95->GetYaxis()->SetTitle("Cumulative Covid-19 cases");

		grint95->GetXaxis()->SetRangeUser(1,30);
		grint95->GetYaxis()->SetRangeUser(0,1000);

		TCanvas* c = new TCanvas("Covid19-MAR","Covid19-MAR",750,750);

		grint95->Draw("ape3");

		grint68->SetLineColor(kRed);
		grint68->SetFillColorAlpha(3, 1.);
		grint68->Draw("e3same");
		
		gr->Draw("psame");

		auto legend = new TLegend(0.1,0.8,0.4,0.9);
		legend->AddEntry(gr,"Data");
		legend->AddEntry(grint95, "99.99% Conf. Int. (#pm 5#sigma)");
		legend->AddEntry(grint68, "95% Conf. Int. (#pm 2#sigma)");
		legend->AddEntry(f_l, Form("Fit, #chi2/ndf = %.1f",f_l->GetChisquare()/f_l->GetNDF()));
		legend->Draw("same");

		auto X    = grint95->GetX();
		auto Y    = grint95->GetY();
		auto EY95 = grint95->GetEY();
		auto EY68 = grint68->GetEY();
	
		int p = 700;

		auto t = new TLatex(1,800, Form("%i March 2020", n));

		auto t0 = new TLatex(1,p,"  Day        : Cases #pm 2#sigma #pm 5#sigma");
		auto t1 = new TLatex(1,p-50,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[19], Y[19] , EY68[19] , EY95[19]));
		auto t2 = new TLatex(1,p-100,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[20], Y[20] , EY68[20] , EY95[20]));
		auto t3 = new TLatex(1,p-150,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[21], Y[21] , EY68[21] , EY95[21]));
		auto t4 = new TLatex(1,p-200,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[22], Y[22] , EY68[22] , EY95[22]));
		auto t5 = new TLatex(1,p-250,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[23], Y[23] , EY68[23] , EY95[23]));
		auto t6 = new TLatex(1,p-300,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[24], Y[24] , EY68[24] , EY95[24]));
		auto t7 = new TLatex(1,p-350,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[25], Y[25] , EY68[25] , EY95[25]));
		auto t8 = new TLatex(1,p-400,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[26], Y[26] , EY68[26] , EY95[26]));
		auto t9 = new TLatex(1,p-450,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[27], Y[27] , EY68[27] , EY95[27]));
		auto t10 = new TLatex(1,p-500,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[28], Y[28] , EY68[28] , EY95[28]));
		auto t11 = new TLatex(1,p-550,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[29], Y[29] , EY68[29] , EY95[29]));
		auto t12 = new TLatex(1,p-600,Form("%.0f March : %.0f #pm %.1f #pm %.1f", X[30], Y[30] , EY68[30] , EY95[30]));

		float size = 0.025;

		t->SetTextSize(size);
		t->SetTextColor(4);
		t0->SetTextSize(size);
		t0->SetTextColor(2);
		t1->SetTextSize(size);
		t2->SetTextSize(size);
		t3->SetTextSize(size);
		t4->SetTextSize(size);
		t5->SetTextSize(size);
		t6->SetTextSize(size);
		t7->SetTextSize(size);
		t8->SetTextSize(size);
		t9->SetTextSize(size);
		t10->SetTextSize(size);
		t11->SetTextSize(size);
		t12->SetTextSize(size);

		t->Draw("same");
		t0->Draw("same");
		t1->Draw("same");
		t2->Draw("same");
		t3->Draw("same");
		t4->Draw("same");
		t5->Draw("same");
		t6->Draw("same");
		t7->Draw("same");
		t8->Draw("same");
		t9->Draw("same");
		t10->Draw("same");
		t11->Draw("same");
		t12->Draw("same");

		c->SaveAs(Form("plot_day_%i.png",n));
}



