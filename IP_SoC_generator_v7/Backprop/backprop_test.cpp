#include "backprop.h"

int main() {

	TYPE weights1[input_dimension * nodes_per_layer], weights2[nodes_per_layer
			* nodes_per_layer], weights3[nodes_per_layer * possible_outputs],
			biases1[nodes_per_layer], biases2[nodes_per_layer],
			biases3[possible_outputs], training_data[training_sets
					* input_dimension], training_targets[training_sets
					* possible_outputs];

	FILE         *fp;


	for (int j = 0; j < input_dimension * nodes_per_layer; j++) {
		weights1[j] = 0.3;
	}
	for (int j = 0; j < nodes_per_layer * nodes_per_layer; j++) {
		weights2[j] = 0.3;
	}
	for (int j = 0; j < nodes_per_layer * possible_outputs; j++) {
		weights3[j] = 0.3;
	}
	for (int j = 0; j < nodes_per_layer; j++) {
		biases1[j] = 0.3;
	}
	for (int j = 0; j < nodes_per_layer; j++) {
		biases2[j] = 0.3;
	}
	for (int j = 0; j < possible_outputs; j++) {
		biases3[j] = 0.3;
	}
	for (int j = 0; j < training_sets * input_dimension; j++) {
		training_data[j] = j;
	}
	for (int j = 0; j < training_sets * possible_outputs; j++) {
		training_targets[j] = j;
	}

	backprop(weights1, weights2, weights3, biases1, biases2, biases3,
			training_data, training_targets);


	fp=fopen("out.dat","w");


		for (int i = 0; i < input_dimension * nodes_per_layer; i++) {
			fprintf(fp,"%.3f\n",double(weights1[i]));
		}
		for (int i = 0; i < nodes_per_layer * nodes_per_layer; i++) {
			fprintf(fp,"%.3f\n",double(weights2[i]));
		}
		for (int i = 0; i < nodes_per_layer * possible_outputs; i++) {
			fprintf(fp,"%.3f\n",double(weights3[i]));
		}
		for (int i = 0; i < nodes_per_layer; i++) {
			fprintf(fp,"%.3f\n",double(biases1[i]));
		}
		for (int i = 0; i < nodes_per_layer; i++) {
			fprintf(fp,"%.3f\n",double(biases2[i]));
		}
		for (int i = 0; i < possible_outputs; i++) {
			fprintf(fp,"%.3f\n",double(biases3[i]));
		}
		for (int i = 0; i < training_sets * input_dimension; i++) {
			fprintf(fp,"%.3f\n",double(training_data[i]));
		}
		for (int i = 0; i < training_sets * possible_outputs; i++) {
			fprintf(fp,"%.3f\n",double(training_targets[i]));
		}
		fclose(fp);

		printf("Comparing against output data \n");
			if (system("diff -w out.dat out_gold.dat")) {

				fprintf(stdout, "*******************************************\n");
				fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
				fprintf(stdout, "*******************************************\n");
				return 1;
			} else {
				fprintf(stdout, "*******************************************\n");
				fprintf(stdout, "PASS: The output matches the golden output!\n");
				fprintf(stdout, "*******************************************\n");
				return 0;
			}

}
