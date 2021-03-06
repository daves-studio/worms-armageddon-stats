require(ggplot2)
d <- subset(read.csv("nr.csv"), !(team %in% c("1-UP", "2-UP")))
ggplot(d, aes(ropes)) + geom_histogram() + facet_wrap(~team, scales="free_y")
ggplot(d, aes(ropes, other_damage-self_damage)) + geom_jitter(alpha=0.2) + facet_wrap(~team) + ylim(-100, 400)
ggplot(d, aes(other_damage-self_damage)) + geom_histogram() + xlim(-200, 400) + facet_wrap(~team, scales="free_y")