#include "engineimp.h"
#include "engine.h"
#include <iostream>
#include "utils/comp_funcs.h"
#include "rhs/rhs.h"
#include "integrator/integrator.h"
#include "controller/controller.h"
#include <time.h>
#include <stdio.h>
void make_torxyz(double c1, double c2, double rb, double rs, double* out){
    out[0]=cos(c1)*(cos(c2)+rb);
    out[1]=sin(c1)*(cos(c2)+rb);
    out[2]=sin(c2);
}

void rmat(comp* in, double alpha){
    in[0] = cos(alpha);
    in[1] = -1*sin(alpha);
    in[2]=sin(alpha);
    in[3]=cos(alpha);
}
void jac(comp* jm, comp* x, comp* store, comp* store2, rhs* rh){
    for(size_t i = 0; i < rh->dimension; i++)
    {
        const double diff = .0001;
        x[i] += diff;
        rh->dxdt(x, store, 0);
        x[i] -= diff*2;
        rh->dxdt(x, store, 0);
        for(size_t j = 0; j < rh->dimension; j++){
            jm[j+rh->dimension*i]=(store[j]-store2[j])/(2*diff);
        }
    };
}
void engineimp::run(){
    int ints;
    values["dimension"]->retrieve(&ints);
    size_t nts=(size_t)ints;
    nts/= 2;
    double t_int;
    values["t_int"]->retrieve(&t_int);
    if(!values.count("integrator")){
        err("integrator variable is not defined", "engineimp::run()",
                "parser/engine_run.cpp", FATAL_ERROR);
    }
    if(!values.count("rhs")){
        err("rhs variable is not defined", "engineimp::run()",
                "parser/engine_run.cpp", FATAL_ERROR);
    }
    integrator* inter;
    values["integrator"]->retrieve(&inter);
    rhs* rh;
    values["rhs"]->retrieve(&rh);
    comp* u0 = (comp*)al_malloc(2*nts*sizeof(comp));
    comp* u1 = (comp*)al_malloc(2*nts*sizeof(comp));
    double* t = (double*)al_malloc((nts)*sizeof(double)); 
    double dt =t_int*1.0/nts;
    comp m1[4], m2[4], m3[4], m4[4];
    Eigen::Map<Eigen::Matrix<comp, 2, 2, Eigen::RowMajor> > em1(m1);
    Eigen::Map<Eigen::Matrix<comp, 2, 2, Eigen::RowMajor> > em2(m2);
    for(int i = 0; i < nts; i++){
        t[i] = dt*((double)i-nts/2.0);
    }
    for(int i = 0; i < nts; i++){
        u0[i] = u0[i+nts] = 1.00/cosh(t[i]/2.0);
    }
    FILE* f = fopen("gnuout", "w");
    fftw_plan t2 = fftw_plan_dft_1d(nts, u0+nts, u0+nts, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan t1 = fftw_plan_dft_1d(nts, u0, u0, FFTW_BACKWARD, FFTW_ESTIMATE);
    fft(t1, u0, u0, nts);
    fft(t2, u0+nts, u0+nts, nts);
    clock_t tval =clock();
   
    rh->dxdt(u0, u1, 1);
    tval = clock()-tval;
    std::cout<<"time for rhs calls="<<tval<<std::endl;
    Eigen::Map<Eigen::Matrix<comp, 2, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Aligned> dmap(u0, 2, nts*2);
    double a1, a2, a3, ap;
    values["a1"]->retrieve(&a1);
    values["a2"]->retrieve(&a2);
    values["a3"]->retrieve(&a3);
    values["ap"]->retrieve(&ap);
    controller* cont;
    values["cont"]->retrieve(&cont);
    double xyz[3];
    for(int i = 0; i < 180; i++){

        for(int j = 0; j < nts*2; j++){
            u1[j] = u0[j];
        }
        make_torxyz(a1, ap, 3, 1, xyz);
        fprintf(f, "%.3lf, %.3lf, %.3lf\n", xyz[0], xyz[1], xyz[2]);
        cont->control(0, 0);
        // inter->integrate(rh, u1, 0, 1.5);

        //std::cout << "done" << std::endl;
        //       rh->dxdt(u0, u1, 0);
        //     rh->dxdt(u1, u0, 0);

    }
    fprintf(f, "e");
    fclose(f);
    // ifft(t1, u0, u0, nts);
    //  ifft(t1, u0+nts, u0+nts, nts);
    ifft(t1, u1, u1, nts);
    ifft(t1, u1+nts, u1+nts, nts);
    for(int i = 0; i < nts*2; i++){
        //std::cout<<_real(u1[i])<<"+"<< _imag(u0[i])<<"i\n";
        //     cout << _sqabs(u1[i]) << endl;
    }
    double ener = energy(u1, nts*2)*t_int/nts;
    std::cout << "Energy is: " << ener << std::endl;
    //std::cout << ((double)clock() - tval)/CLOCKS_PER_SEC << std::endl;
    fftw_destroy_plan(t2);
    fftw_destroy_plan(t1);
    al_free(u0);
    al_free(u1);
    al_free(t);
    fftw_cleanup();
}
