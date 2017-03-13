//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include <iostream>
#include <chrono>

#include "dll/rbm/rbm.hpp"
#include "dll/trainer/stochastic_gradient_descent.hpp"
#include "dll/dbn.hpp"

#include "mnist/mnist_reader.hpp"
#include "mnist/mnist_utils.hpp"

// 1 3-layer networks

int main(int, char**) {
    auto dataset = mnist::read_dataset_direct<std::vector, etl::dyn_vector<float>>();

#define decl_dbn3(NAME, NAME_T, F)                                                                                              \
    using NAME_T =                                                                                                              \
        dll::dbn_desc<                                                                                                          \
            dll::dbn_layers<                                                                                                    \
                dll::rbm_desc<28 * 28, 100 + F, dll::momentum, dll::batch_size<64>>::layer_t,                                   \
                dll::rbm_desc<100 + F, 200 + F, dll::momentum, dll::batch_size<64>>::layer_t,                                   \
                dll::rbm_desc<200 + F, 10, dll::momentum, dll::batch_size<64>, dll::hidden<dll::unit_type::SOFTMAX>>::layer_t>, \
            dll::trainer<dll::sgd_trainer>, dll::batch_size<64>>::dbn_t;                                                        \
    auto NAME = std::make_unique<NAME_T>();                                                                                     \
    NAME->pretrain(dataset.training_images, 20);                                                                                \
    NAME->fine_tune(dataset.training_images, dataset.training_labels, 20);

    decl_dbn3(dbn1,dbn1_t,1)

    return 0;
}
