#include <hc.hpp>

using namespace hc;


void single_view_inorder(int* da, int* db, const unsigned int num) {
  accelerator acc = accelerator();
  accelerator_view av = acc.create_view(queuing_mode::queuing_mode_in_order);

  parallel_for_each(av, extent<1>(num), [=](index<1> idx) {
    da[idx[0]] += db[idx[0]]; 
  });

  // executes after the previous kernel has been completed, no race condition
  completion_future cf = parallel_for_each(av, extent<1>(num), [=](index<1> idx) {
    db[idx[0]] ++; 
  });

  cf.wait();
  // av.wait(); /* alternatively, wait on the accelerator_view*/
}


void multiple_view_inorder(int* da, int* db, const unsigned int num) {
  accelerator acc = accelerator();
  accelerator_view av1 = acc.create_view(queuing_mode::queuing_mode_in_order);
 
  completion_future cf1 = parallel_for_each(av1, extent<1>(num), [=](index<1> idx) {
    da[idx[0]] += db[idx[0]]; 
  });


  // enqueue the 2nd kernel to a different accelerator_view and use completion to synchronize
  accelerator_view av2 = acc.create_view(queuing_mode::queuing_mode_in_order);
  vector<completion_future> cfs;
  cfs.push_back(cf1);
  completion_future cf2 = barrier(av2, cfs, parallel_for_each(av, extent<1>(num)
                                                              , [=](index<1> idx) {
                                                                db[idx[0]] ++; 
                                                              }));
  cf2.wait();
  // av2.wait(); /* alternatively, wait on the accelerator_view*/
}





void single_view_outoforder(int* da, int* db, const unsigned int num) {
  accelerator acc = accelerator();

  // out-of-order queue
  accelerator_view av = acc.create_view(queuing_mode::queuing_mode_out_of_order);
 
  completion_future cf1 = parallel_for_each(av, extent<1>(num), [=](index<1> idx) {
    da[idx[0]] += db[idx[0]]; 
  });


  // enqueue the 2nd kernel and use completion to synchronize
  vector<completion_future> cfs;
  cfs.push_back(cf1);
  completion_future cf2 = barrier(av, cfs, parallel_for_each(av, extent<1>(num)
                                                              , [=](index<1> idx) {
                                                                db[idx[0]] ++; 
                                                              }));
  cf2.wait();
  // av.wait(); /* alternatively, wait on the accelerator_view*/
}




void single_view_outoforder_independent_kernels(int* da, int* db, const unsigned int num) {
  accelerator acc = accelerator();

  // out-of-order queue
  accelerator_view av = acc.create_view(queuing_mode::queuing_mode_out_of_order);
 
  parallel_for_each(av, extent<1>(num), [=](index<1> idx) {
    da[idx[0]] ++; 
  });

  // no dependency to the previous kernel, both kernels could be run in parallel with out-of-order queuing
  parallel_for_each(av, extent<1>(num), [=](index<1> idx) {
    db[idx[0]] ++; 
  });

  av.wait(); 
}





void multiple_view_inorder_with_copy(int* da, int* db, int* dc, const unsigned int num) {
  accelerator acc = accelerator();

  // out-of-order queue
  accelerator_view av1 = acc.create_view(queuing_mode::queuing_mode_in_order);
 
  parallel_for_each(av1, extent<1>(num), [=](index<1> idx) {
    da[idx[0]] ++; 
  });

  // copy from da to db with the first array_view
  completion_future cf1 = copy(av1, da, db);

  accelerator_view av2 = acc.create_view(queuing_mode::queuing_mode_in_order);
  vector<completion_future> cfs;
  cfs.push_back(cf1);
  completion_future cf2 = barrier(av2, cfs, parallel_for_each(av, extent<1>(num)
                                                              , [=](index<1> idx) {
                                                                dc[idx[0]] = db[idx[0]]; 
                                                                }));

  // wait for the 2nd kernel to complete. da,db,dc should have the same content.
  cf2.wait();
}


void single_view_out_of_order_with_copy(int* da, int* db, int* dc, const unsigned int num) {
  accelerator acc = accelerator();

  // out-of-order queue
  accelerator_view av = acc.create_view(queuing_mode::queuing_mode_out_of_order);
 
  completion_future cf1 = parallel_for_each(av, extent<1>(num), [=](index<1> idx) {
    da[idx[0]] ++; 
  });

  vector<completion_future> cfs;
  cfs.push_back(cf1);

  // wait for the 1st kernel to complete and then copy from da to db
  barrier(av, cfs, copy(av, da, db));

  // wait for the 1st kernel to complete and then run the pfe
  // notice the 2nd kernel and the previous buffer copy could run in parallel
  barrier(av, cfs, parallel_for_each(av, extent<1>(num)
                                     , [=](index<1> idx) {
                                           dc[idx[0]] = da[idx[0]];  
                                     }));

  // wait for everything to be done. da, db, dc should have the same content
  av.wait(); 
}


