library('ggplot2')
library('reshape2')
library('reshape')

source("common.R")

display_names = list(gaussian="NFGE (gaussian)",
                     box="NFGE (box)",
                     constant="Constant")

data = data.frame()
for(name in c("gaussian", "box", "constant")) {
    tmp = read.csv(paste(name, ".csv", sep=""))
    tmp = cbind(name=display_names[[name]],tmp)
    data = rbind(data, tmp)
}



data_plot = melt(data, id.vars=c('X', 'train.size', 'name'))
data_plot = data_plot[which(data_plot$variable %in% c("abs.mean")),]


levels(data_plot$variable)[which(levels(data_plot$variable) == "abs.mean")] = "Average"
#levels(data_plot$variable)[which(levels(data_plot$variable) == "abs.sd")] = "Standard deviation"


p = baseplot(data_plot, aes(train.size, value,
                            group=name,
                            color=name,
                            shape=name))
p = p + my.geom_line()
p = p + geom_point()
p = p + my.scale_color_discrete(name = "Setup")#"MHS Algorithm")
p = p + my.scale_shape_discrete(name = "Setup")#"MHS Algorithm")
p = p + scale_x_continuous(breaks=sort(unique(data_plot$train.size)),
                           name = "Training size")
p = p + scale_y_continuous(name = "Average Error", limits=c(0.1,0.6),breaks=c(0.1,0.2,0.3,0.4,0.5,0.6))

open.pdf("synthetic_results.pdf", width=4, height=3)
print (p)
dev.off()
