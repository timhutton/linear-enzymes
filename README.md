# linear-enzymes
Artificial chemistry where chains of atoms can catalyse reactions. Unpublished.

[A live demo](https://timhutton.github.io/linear-enzymes/enzymes_demo/) shows a short molecule being replicated by 21 free floating enzymes:

<a href="https://timhutton.github.io/linear-enzymes/enzymes_demo/"><img width="600px" src="https://user-images.githubusercontent.com/647092/170825245-5c55b047-5961-4029-9b82-0c988e6377f9.gif"></a>

Compare with [https://github.com/timhutton/squirm3](https://github.com/timhutton/squirm3) where the reactions just happen magically - they are hard-coded into the simulation. Here the enzymes carry the information about each reaction. And in contrast to some previous work the enzymes are made of the same range of states as the rest of the world and can be assembled and manipulated in the same way. The simulation only provides a rule that reads the sequence of atoms in a linear chain and applies the corresponding reaction locally.

[Another demo](https://timhutton.github.io/linear-enzymes/compaction_demo/) shows how allowing multiple atoms to occupy a square under some circumstances can reduce the computation needed and allow more flexibility:

<a href="https://timhutton.github.io/linear-enzymes/compaction_demo/"><img width="600px" src="https://user-images.githubusercontent.com/647092/173341688-00ae35e4-a484-417e-9740-dda21eb5ce2b.png"></a>
