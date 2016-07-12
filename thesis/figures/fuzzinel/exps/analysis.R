library('ggplot2')
library('reshape2')
library('reshape')
library('gdata')
source('common.R')
library(extrafont)

read.data = function(path, generator.rename=NULL, ranker.rename=NULL) {
    data = read.csv(path, strip.white=T)

  if(length(generator.rename))
    data$generator = factor(data$generator, labels=generator.rename)
  if(length(ranker.rename)) {
    data$ranker = factor(data$ranker, labels=ranker.rename)
  }

  casted_data = cast(data, system + iteration + generator + ranker ~metric_name)
  colnames(data) = trim(colnames(data))
  colnames(casted_data) = trim(colnames(casted_data))


  return (list(casted=casted_data, melted=data))
}


get.data = function(data, rankers=NULL, generators=NULL, randomizers=NULL) {
  elements = 1:nrow(data)

  if(length(rankers))
    elements = intersect(elements, which(data$ranker %in% rankers))

  if(length(generators))
    elements = intersect(elements, which(data$generator %in% generators))

  if(length(randomizers))
    elements = intersect(elements, which(data$randomizer %in% randomizers))


  return (data [elements, ])

}

boom.selectah = function(data, randomizers=NULL){
  if(length(randomizers)) {
    elements = which(data$randomizer %in% randomizers)
    data = data[elements,]
  }


  data = data[!is.nan(data$value),]

  casted_data = cast(data, generator + ranker ~metric_name, mean, rm.na=T)
  return (casted_data)
}

boom.selectah.plot = function(data) {
  data = data [which(!is.na(data$value)), ]

  p = qplot(value, data=data, geom="density", fill=ranker, alpha=I(.5))
  p = p + facet_wrap(~metric_name, scales="free")

  return(p)
}

#setwd('~/work/phd/libdiag/AAAI/setup3/')
#setwd('AAAI/')
summary = data.frame()
full = data.frame()
for(j in 2:4){
    for(i in 1:9){
        setwd(sprintf("%df%de", j, i))
        a = read.data('metrics.csv')
        tmp_full=a$casted
        tmp_summary = boom.selectah(a$melted)
#
        tmp_full=cbind(data.frame(nerr=i, nfault=j), tmp_full)
        tmp_summary=cbind(data.frame(nerr=i, nfault=j), tmp_summary)
#
        full = rbind(full,tmp_full)
        summary = rbind(summary,tmp_summary)
#
        setwd("..")
    }
}


full[is.na(full)] = 0

nfault_labeller <- function(s) {
    for (x in 1:ncol(s)){
    print(x)
    if(colnames(s)[x] == "nfault")
        s[,x] = sprintf("# Faults: %d", s[,x])
    }
    return (s)
}


## y_labeller <- function(x) {
##     return (sprintf("%g\%", x*100))
## }

y_labeller = my.percent

plot_quality_vs_nerr_nfault = function(data) {
    p = baseplot(data, aes(nerr, wasted_effort_quality_fair,color=ranker, shape=ranker))
    p = p + geom_point()
    p = p + geom_line()
    p = p + scale_y_continuous(limits=c(0,1), label=y_labeller)
    p = p + facet_grid(nfault~., labeller=nfault_labeller)
    p = p + my.scale_color_discrete(name = "Diagnostic Type", labels = c("fuzzinel" = "Fuzzy", "barinel" = "Crisp"))
    p = p + my.scale_shape_discrete(name = "Diagnostic Type", labels = c("fuzzinel" = "Fuzzy", "barinel" = "Crisp"))
    p = p + scale_x_continuous(breaks = as.numeric(labels(factor(data$nerr))))
    p = p + ylab("Diagnostic Quality")
    p = p + xlab("Number of Errors")
    p = p + labs(shape="Number of Faults", colour="Diagnosis",linetype="Number of Faults")
    return (p)
}

plot_quality_vs_nerr_nfault_boxplot = function(data) {
    p = baseplot(data,
        aes(factor(nerr),
            wasted_effort_quality_fair,
            fill=factor(ranker),
            color=factor(ranker)
            ))
    p = p + scale_y_continuous(limits=c(0,1), label=y_labeller)
    p = p + geom_boxplot(outlier.shape="-", colour="gray0")
    p = p + my.scale_fill_discrete(name = "Diagnostic Type", labels = c("fuzzinel" = "Fuzzy", "barinel" = "Crisp"))
    p = p + facet_grid(nfault ~ ., labeller=nfault_labeller)
    p = p + ylab("Diagnostic Quality")
    p = p + xlab("Number of Errors")
    p = p + labs(shape="Number of Faults", fill="Diagnosis",linetype="Number of faults")
    return (p)
}

plot_quality_vs_nerr_nfault_jitter = function(data) {
    p = baseplot(data,
        aes(factor(ranker),
            wasted_effort_quality_fair,
            fill=factor(ranker),
            color=factor(ranker)
            ))

    p = p + scale_y_continuous(limits=c(-0.1,1), label=y_labeller)
    p = p + my.scale_fill_discrete(name = "Diagnostic Type", labels = c("fuzzinel" = "Fuzzy", "barinel" = "Crisp"))
    p = p + my.scale_color_discrete(name = "Diagnostic Type", labels = c("fuzzinel" = "Fuzzy", "barinel" = "Crisp"))
    p = p + geom_jitter(alpha=0.05, shape=1,position = position_jitter(h=0,w=0.2))
    p = p + scale_x_discrete("Number of Errors", labels = c("fuzzinel" = "", "barinel" = ""))
    p = p + facet_grid(nfault ~ nerr, labeller=nfault_labeller,switch="x")
    p = p + ylab("Diagnostic Quality")
    p = p + xlab("Number of errors")
    p = p + labs(shape="Number of faults", fill="Diagnosis",linetype="Number of faults")
    p = p + guides(colour = guide_legend(override.aes = list(alpha = 1)))
    p = p + theme(axis.ticks.x = element_blank(),
                  axis.line.x = element_blank(),
                  axis.text.x = element_blank(),
                  strip.switch.pad.grid =unit(-0, "cm"))


    return (p)
}

plot_differences_density = function(data){
    barinel = full$wasted_effort_quality_fair[which(full$ranker == 'barinel')]
    fuzzinel = full$wasted_effort_quality_fair[which(full$ranker != 'barinel')]

    diff = data.frame(x=fuzzinel-barinel)

    total = nrow(diff)
    fuz_better = length(which(diff$x>0))
    bar_better = length(which(diff$x<0))

    mean_diff = mean(diff$x)


    p = baseplot(diff, aes(x))

    tmp = density(diff$x)
    tmp = data.frame(x=tmp$x, y=tmp$y)
    p = p + geom_area(data = subset(tmp, x >= 0),
        aes(x=x,y=y), fill = defaultPalette[2])
    p = p + geom_area(data = subset(tmp, x <= 0),
        aes(x=x,y=y), fill = defaultPalette[1])
    p = p + geom_density()

    p = p + geom_vline(xintercept = mean_diff, linetype=2, show_guide = T)
    p = p + ylim(0,6)
    #p = p + annotate("text", x = 0.6, y = 4.5, size=3, label =
    #    sprintf("Average: %.3f", round(mean_diff,3)))




    #p = p + annotate("text", x = 0.6, y = 3.2, size=3, label =
    #    sprintf("Fuzzy > Crisp: %d%%", round(fuz_better / total * 100)))

    #p = p + annotate("text",  x = 0.6, y = 2.8, size=3, label =
    #    sprintf("Fuzzy >= Crisp: %d%%", 100 - round(bar_better / total * 100)))

    #p = p + annotate("text", x = -0.45, y = 3, size=3, label =
    #    sprintf("Fuzzy < Crisp: %d%%", round(bar_better / total * 100)))

    p = p + theme(axis.title.x = element_blank())
    p = p + ylab("Density")

    return (p)
}

#print(plot_differences_density(full))



open.pdf("plot1.pdf", bg="transparent", width=5,height = 3.5)
print(plot_differences_density(full))
dev.off()

open.pdf("plot2.pdf", bg="transparent", width=2.9,height = 7)
print(plot_quality_vs_nerr_nfault(summary))
dev.off()

open.pdf("plot3.pdf", bg="transparent", width=3.9,height = 7)
print(plot_quality_vs_nerr_nfault_boxplot(full))
dev.off()

open.pdf("plot4.pdf", bg="transparent", width=5,height = 7)
print(plot_quality_vs_nerr_nfault_jitter(full))
dev.off()



head(full)

#T-test
barinel = full$wasted_effort_quality_fair[which(full$ranker == 'barinel')]
fuzzinel = full$wasted_effort_quality_fair[which(full$ranker != 'barinel')]

plot(density(fuzzinel - barinel))
abline(v=mean(fuzzinel) - mean(barinel))
abline(v=0)



xxx = fuzzinel - barinel
keeps = c("nerr", "nfault", "system", "iteration", "wasted_effort_quality_fair", "wasted_effort")
barinel2 = full[which(full$ranker == 'barinel'),keeps][which(xxx<0),]
fuzzinel2 = full[which(full$ranker != 'barinel'),keeps][which(xxx<0),]




#all(which(xxx > 0) == which(xxx2 < 0))


write.csv(barinel2, "barinel.csv")
write.csv(fuzzinel2, "fuzzinel.csv")


var.test(fuzzinel, barinel)

t.test(fuzzinel, barinel, alternative = "greater", conf.level = 0.99, paired = F)

improvement = function(x,y){
    return ((x-y) / (y))
}


barinel = summary[which(summary$ranker == 'barinel'),]
fuzzinel = summary[which(summary$ranker != 'barinel'),]


hist(improvement(fuzzinel$wasted_effort_quality_fair,
                barinel$wasted_effort_quality_fair))

improvement(fuzzinel$wasted_effort_quality_fair,
                barinel$wasted_effort_quality_fair)


# baseplot(tmp, aes(wasted_effort_quality_fair, fill = nfault)) +
#  stat_density(aes(y = ..density..), position = "identity", color = #"black", alpha = 0.5)
