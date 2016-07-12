source("process.R")
source("process_parallel.R")
data_small_avg = read.csv(get.csv.path("optim_small_avg.csv"))
p_small = plot.optim.small(data_small_avg)
p_small2 = plot.optim.small2(data_small_avg)

data_large_avg = read.csv(get.csv.path("optim_large_avg.csv"))
p_large = plot.optim.large(data_large_avg)

open.pdf("optim_legend.pdf", width=5, height=6)
print (p_small[[1]])
dev.off()
open.pdf("optim_small.pdf", width=5, height=6.5)
print (p_small)
dev.off()

open.pdf("optim_small2.pdf", width=5, height=3)
print (p_small2)
dev.off()

open.pdf("optim_large.pdf", width=5, height=2.5)
print (p_large)
dev.off()

data_all = read.csv(get.csv.path("parallel_small_all.csv"))
data_avg = read.csv(get.csv.path("parallel_small_avg.csv"))
p_small = plot.parallel.small(data_all, data_avg)

open.pdf("parallel_small.pdf", width=5, height=2.7)
print(p_small)
dev.off()

data = read.csv(get.csv.path("parallel_large_avg.csv"))
p_large = plot.parallel.large(data)

open.pdf("parallel_large.pdf", width=5, height=3.5)
print(p_large)
dev.off()


data = read.csv(get.csv.path("parallel_large_avg.csv"))
p_large_scatter = plot.parallel.large.scatter(data)
open.pdf("parallel_scatter_large.pdf", width=5, height=3)
print(p_large_scatter)
dev.off()
