###MovementOfTheMaximum
Author:EricLin
Email:463222898@qq.com

###Email to the paper's author
---
Referring to the paper:Memory Enhanced Evolutionary Algorithms for Changing Optimization Problems, Jurgen Branke.

I find that the Figure2(The movement of the maximum through a 2-dimensional subspace over 300 changess, shifting vector v has length s=0.9) in the above paper, doesn't seem follow the change equation in the paper.

What I implement it carefully, and with the exactly parameters, I get pictures different from the Figure2 in the paper, in some ways.

I find out , that the maximum will stay more often at some peaks, not averaging jumping over all the 5 peaks.

So I would like to share my code, and see if I have misunderstood the meaning of the papepr's author, or I have made a mistake in my code.

Thanks in advance~

---
###About files
main.cpp:
Generates data.

drawPath.py:
draw graph base on data.

figure_*.png:
Generated pictures.

---
###How to build it?
run `make`

