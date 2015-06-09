#define __CL_ENABLE_EXCEPTIONS
// C++ OpenCL API
#include <CL/cl.hpp>
// clBLAS and OpenCL headers
#include <clBLAS.h>
// armadillo headers for handling the R input data
#include <RcppArmadillo.h>

#include <bigmemory/MatrixAccessor.hpp>

#include "arma_helpers.hpp"
#include "cl_helpers.hpp"

using namespace Rcpp;

// can add more arguments for more control of sgemm call
// e.g. if transpose needed?

//[[Rcpp::export]]
void cpp_amdBigMatrix_saxpy(SEXP alpha_, SEXP A_, SEXP B_)
{
    
    const cl_float alpha = as<cl_float>(alpha_);
    static const int incx = 1;
    static const int incy = 1;

    const arma::Mat<float> Am = ConvertBMtoArma<float>(A_);
    arma::Mat<float> Bm = ConvertBMtoArma<float>(B_);
                              
    // total number of elements
    const int N = Am.n_elem;
    
    // declare OpenCL objects
    cl_int err;
    
    /* Setup OpenCL environment. */
    
    // Get available platforms
    std::vector<Platform> platforms;
    getPlatforms(platforms); // cl_helpers.hpp      
    
    // Select the default platform and create a context using this platform and the GPU
    cl_context_properties cps[3] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)(platforms[0])(),
        0
    };

    Context context = createContext(CL_DEVICE_TYPE_GPU, cps, err);
            
    // Get a list of devices on this platform
    std::vector<Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    
    // Create a command queue and use the first device
    CommandQueue queue = CommandQueue(context, devices[0], 0, &err);
    
    /* Setup clblas. */
    err = clblasSetup();
    if (err != CL_SUCCESS) {
        stop("clblasSetup() failed with " + err);
    }
    
    
    /* Prepare OpenCL memory objects and place matrices inside them. */
    Buffer bufA = Buffer(context, CL_MEM_READ_ONLY, N * sizeof(float), NULL, &err);
    Buffer bufB = Buffer(context, CL_MEM_READ_WRITE, N * sizeof(float), NULL, &err);
    
    // Copy lists A and B to the memory buffers
    queue.enqueueWriteBuffer(bufA, CL_TRUE, 0, N * sizeof(float), &Am[0]);
    queue.enqueueWriteBuffer(bufB, CL_TRUE, 0, N * sizeof(float), &Bm[0]);
                          
    /* Call clblas extended function. Perform gemm */
    err = clblasSaxpy(N, alpha, bufA(), 0, incx,
                         bufB(), 0, incy, 1,
                         &queue(), 0, NULL, 0);
    if (err != CL_SUCCESS) {
//        std::cout << err << std::endl;
        stop("clblasSaxpy() failed");
    }
    else {
        
        /* Wait for calculations to be finished. */                             
//        err = clEnqueueReadBuffer(queue, bufB, CL_TRUE, 0,
//                                  (N * sizeof(Bm[0])),
//                                  &Bm[0], 0, NULL, NULL);
        err = queue.enqueueReadBuffer(bufB, CL_TRUE, 0, 
                                    N * sizeof(float), 
                                    &Bm[0]);
    }
    
    /* Finalize work with clblas. */
    clblasTeardown();
}
