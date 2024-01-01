#include <stdio.h>
#include <stdlib.h>
#include "CS-1203/headers/Egraph.h"

typedef struct {
  ENode* source[MAX_S_EXPRS];
  Map map[MAX_S_EXPRS];
} Rule;

bool compatible(Tensor* targetTensor, Tensor* matchTensor) {
    for (int i = 0; i < MAX_DIMENSIONS; ++i) {
        if (targetTensor->shape[i] != -1 && matchTensor->shape[i] != -1 &&
            targetTensor->shape[i] != matchTensor->shape[i]) {
            return false;
        }
    }
    return true;
}

EGraph* rewrite(EGraph* G, Rule* Rm[], int ruleCount, int MAX_ITER) {
    EGraph* ec = buildEGraph(0);

    // ec is our canonical set
    for (int i = 0; i < ruleCount; ++i) {
        for (int j = 0; j < MAX_S_EXPRS; ++j) {
            Map renameMap;
            ENode* e = Rm[i]->source[j];
            add(ec, e);
            Rm[i]->map[j] = renameMap;
        }
    }

    // Tensat exploration phase
    for (int iter = 0; iter < MAX_ITER; ++iter) {
        MatchList M = search(G, ec);

        for (int i = 0; i < ruleCount; ++i) {
            for (int j = 0; j < MAX_S_EXPRS; ++j) {
                MatchList mci = M[Rm[i]->source[j]];
                MatchList mi = decanonical(mci, &(Rm[i]->map[j]));
            }

            for (int k = 0; k < mi.count; ++k) {
                if (compatible(&(mi.matches[k]))) {
                    apply(G, Rm[i], mi.matches[k].sigma0, ...);
                }
            }
        }
    }

    return G;
}

void matMul(EGraph* G, int k) {
    for (int iter = 0; iter < k; ++iter) {
        for (int i = 0; i < G->size; ++i) {
            ENode* matmul1 = G->nodes[i];
            for (int j = i + 1; j < G->size; ++j) {
                ENode* matmul2 = G->nodes[j];
                if (matmul1->input1 == matmul2->input1) {
                    ENode* concat2 = createConcat2Node(1, matmul1->input2, matmul2->input2);
                    ENode* matmulNew = createMatmulNode(matmul1->input1, concat2);
                    ENode* split0 = createSplitNode(1, 1, matmulNew);
                    ENode* split1 = createSplitNode(1, 1, matmulNew);
                    G->nodes[G->size++] = matmulNew;
                    G->nodes[G->size++] = split0;
                    G->nodes[G->size++] = split1;
                }
            }
        }
    }
}

int main() {

    EGraph* G = createEGraph();
    Rule* Rm[MAX_RULES];
    int ruleCount = 2;  
    int MAX_ITER = 10;  

    EGraph* result = rewrite(G, Rm, ruleCount, MAX_ITER);

    return 0;
}

