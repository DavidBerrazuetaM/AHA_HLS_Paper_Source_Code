/*******************************************************************************
 Source: MachSuite (please look the LICENSE below for details)
 Modified by: David Berrazueta & Byron Navas
 Associated Filenames: fcn.cpp, hd.h, tb.cpp, gold.dat
 Purpose: BPNN software version
 Revision History: November 11, 2019
 *******************************************************************************
 BPNN v0.1
 *******************************************************************************
 BSD-3 License (backprop implementation)

Copyright (c)2008-2011 University of Virginia
Copyright (c) 2014-2015, the President and Fellows of Harvard College
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted without royalty fees or other restrictions, provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
* Neither the name of the University of Virginia, the Dept. of Computer Science,
  nor the names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY OF VIRGINIA OR THE SOFTWARE AUTHORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#define input_dimension  13
#define possible_outputs  3
#define training_sets   163
#define nodes_per_layer  64
#define layers            2
#define learning_rate  0.01
#define epochs            1
#define test_sets        15
#define norm_param    0.005
#define max 1.0
#define offset 0.5
#define MAX 1000
#define MIN 1

typedef double TYPE;

void backprop(
    TYPE weights1[input_dimension*nodes_per_layer],
    TYPE weights2[nodes_per_layer*nodes_per_layer],
    TYPE weights3[nodes_per_layer*possible_outputs],
    TYPE biases1[nodes_per_layer],
    TYPE biases2[nodes_per_layer],
    TYPE biases3[possible_outputs],
    TYPE training_data[training_sets*input_dimension],
    TYPE training_targets[training_sets*possible_outputs]);
