//g++ -std=c++17 -O2 -I include 05_Multigrid.cpp src/Multigrid.cpp -o mgstudy./mgstudy

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cmath>
#include <string>
#include "Multigrid.h"

namespace fs = std::filesystem;

// Convert smoother enum to a string
static std::string smootherName(MultigridLap::Smoother s)
{
    return (s == MultigridLap::GAUSS_SEIDEL) ? "GS" : "JACOBI";
}

static int maxLevels(int N)
{
    int levels = 1;
    int n = N;
     while (n > 2)
    {
        n /= 2;
        levels++;
    }
    return levels;
}
int main()
{
    // Create results folder if it does not already exist
    fs::create_directories("results");
    const std::vector<int> Ns = {16, 32, 64, 128};

    //2 smoothers
    const std::vector<MultigridLap::Smoother> smoothers =
    {
        MultigridLap::GAUSS_SEIDEL,MultigridLap::JACOBI
    };

   // Solver parameters
    // Number of pre and post smoothing sweeps
    const int nu1 = 2;
    const int nu2 = 2;

    // Multigrid convergence tolerance
    const double tolMG = 1.0e-10;

    // Maximum number of multigrid V-cycles
    const int maxCycleMG = 100;

    // Single-grid convergence tolerance
    const double tolSG = 1.0e-10;

    // Maximum number of single-grid iterations
    const int maxIterSG = 500000;
    std::cout
        << "=== PART A: grid convergence "
        << "(multigrid, Gauss-Seidel) ===\n";

    struct ErrRow
    {
        int N;double h;double L2;double Linf;
    };
    std::vector<ErrRow> errRows;
    // Loop over all four grids
    for (int N : Ns)
    {
        MultigridLap solver(N);
    // Use full-depth multigrid
        int levels = maxLevels(N);
        solver.solve(levels,MultigridLap::GAUSS_SEIDEL,nu1,nu2,tolMG,maxCycleMG);
        // Compute errors
        double L2 = solver.computeL2Error();
        double Linf = solver.computeLinfError();
        double h = 1.0 / N;
        // Store results
        errRows.push_back(
            {
                N,h,L2,Linf
            }
        );
        std::cout<< "  N=" << N<< "  levels=" << levels<< "  L2=" << L2<< "  Linf=" << Linf<< "\n";

        // Write complete numerical solution
        std::ofstream f("results/solution_N"+ std::to_string(N) + ".csv");
        f << "x,numerical,exact,abs_error\n";

        for (int i = 0; i <= N; i++)
        {
            double x = solver.getX(i);
            double num = solver.getU(i);
            double exact = MultigridLap::analytical(x);
            double absError = std::abs(num - exact);
            f << x<< "," << num<< "," << exact<< "," << absError<< "\n";
        }
    }

    {
        std::ofstream f("results/error_summary.csv");

        f << "N,h,L2,Linf,L2_order,Linf_order\n";

        for (size_t k = 0; k < errRows.size(); k++)
        {
            double L2order = std::nan("");
            double Linforder = std::nan("");

            // p = log2(error(N) / error(2N))
            if (k > 0)
            {
                L2order =std::log2(errRows[k - 1].L2/errRows[k].L2);
                Linforder =std::log2(errRows[k - 1].Linf/errRows[k].Linf);
            }
            f << errRows[k].N<< "," << errRows[k].h<< "," << errRows[k].L2<< "," << errRows[k].Linf<< ",";

            if (k > 0)
            {
                f << L2order<< "," << Linforder<< "\n";
            }
            else
            {
                f << ","<< "\n";
            }
        }
    }

    std::cout<< "\n=== PART B: residual histories "<< "(single-grid vs multigrid, "<< "Jacobi vs Gauss-Seidel) ===\n";
    // ALL FOUR GRIDS
    const std::vector<int> NsForResiduals =
    {
        16,32,64,128
    };

    // Open convergence summary file
    std::ofstream convSummary("results/convergence_summary.csv");
    convSummary<< "N,smoother,method,iterations,"<< "equivalent_fine_sweeps,"<< "final_residual,converged\n";

    for (int N : NsForResiduals)
    {
        int levels = maxLevels(N);
        //GS and Jacobi
        for (auto sm : smoothers)
        {
            std::string smName = smootherName(sm);
            //multigrid
            {
                MultigridLap solver(N);
                 auto hist =solver.solve(levels,sm,nu1,nu2,tolMG,maxCycleMG);
                // Write residual history

                std::ofstream f("results/residual_multigrid_N"+ std::to_string(N)+ "_"+ smName+ ".csv");
                f << "cycle,residual\n";

                for (size_t i = 0; i < hist.size(); i++)
                {
                    f << (i + 1)<< ","<< hist[i]<< "\n";
                }

                // Convergence information
                bool converged =!hist.empty() &&hist.back() < tolMG;
                // Approximate equivalent fine-grid sweeps
                double equivSweeps =hist.size()* 2.0* (nu1 + nu2);

                convSummary<< N<< "," << smName<< ",multigrid"<< "," << hist.size()<< "," << equivSweeps<< "," << (hist.empty()? 0.0: hist.back())<< "," << (converged ? 1 : 0)<< "\n";
                std::cout<< "  N=" << N<< " " << smName<< " multigrid:"<< " cycles=" << hist.size()<< " finalResidual="<< (hist.empty()? 0.0: hist.back())<< "\n";
            }

            // SINGLE GRID         
            {
                MultigridLap solver(N);auto hist =solver.solveSingleGrid(sm,tolSG,maxIterSG);

                // Write residual history
                std::ofstream f("results/residual_singlegrid_N"+ std::to_string(N)+ "_"+ smName+ ".csv");
                f << "iteration,residual\n";
                for (size_t i = 0; i < hist.size(); i++)
                {
                    f << (i + 1)<< ","<< hist[i]<< "\n";
                }

               // Convergence information
                bool converged =!hist.empty() &&hist.back() < tolSG;

                // One single-grid iteration = one fine-grid sweep
                double equivSweeps =static_cast<double>(hist.size());
                convSummary<< N<< "," << smName<< ",singlegrid"<< "," << hist.size()<< "," << equivSweeps<< "," << (hist.empty()? 0.0: hist.back())<< "," << (converged ? 1 : 0)<< "\n";
                std::cout<< "  N=" << N<< " " << smName<< " single-grid:"<< " iterations=" << hist.size()<< " finalResidual="<< (hist.empty()? 0.0: hist.back())<< "\n";
            }
        }
    }
   
    // Sensitivity to number of multigrid levels
    std::cout<< "\n=== PART C: sensitivity to number "<< "of levels (N=128, Gauss-Seidel) ===\n";
    {
        int N = 128;
        int maxL = maxLevels(N);
        std::ofstream f("results/levels_sensitivity_N128.csv");
        f << "numLevels,coarsestN,cycles,"<< "final_residual,converged\n";
        
        // Try every possible number of levels
        for (int levels = 1;levels <= maxL;levels++)
        {
            MultigridLap solver(N);
            auto hist =solver.solve(levels,MultigridLap::GAUSS_SEIDEL,nu1,nu2,tolMG,maxCycleMG);

            // Determine coarsest grid
            int coarsestN = N;
           for (int l = 0;l < levels - 1;l++)
            {
                coarsestN /= 2;
            }

            bool converged =!hist.empty() &&hist.back() < tolMG;
            f << levels<< "," << coarsestN<< "," << hist.size()<< "," << (hist.empty()? 0.0: hist.back())<< "," << (converged ? 1 : 0)<< "\n";
            std::cout<< "  numLevels=" << levels<< " coarsestN=" << coarsestN<< " cycles=" << hist.size()<< "\n";
        }
    }

    std::cout
        << "\nAll CSV data written under ./results/\n";
    return 0;
}