set terminal png size 800,600
set logscale x 2
set logscale y 2
set format y "%.2e"
set xlabel "Size"
set ylabel "Time(s)"

log2(x) = log(x)/log(2)
AKSDirectLower(x) = a*log2(x)**8
AKSDirectUpper(x) = a*log2(x)**(31/2)
AKSNTLLower(x) = a*log2(x)**6
AKSNTLUpper(x) = a*log2(x)**(21/2)
fit AKSDirectLower(x) "data/aks/direct-mean.txt" via a
fit AKSDirectUpper(x) "data/aks/direct-mean.txt" via a
fit AKSNTLLower(x) "data/aks/ntl-mean.txt" via a
fit AKSNTLUpper(x) "data/aks/ntl-mean.txt" via a
set output "../../Memoria/img/graphs/aks-direct-mean.png"
plot "data/aks/direct-mean.txt" with lines title "AKS Directo", AKSDirectLower(x) title "log^8(n)", AKSDirectUpper(x) title "log^{31/2}(n)"
set output "../../Memoria/img/graphs/aks-ntl-mean.png"
plot "data/aks/ntl-mean.txt" with lines title "AKS NTL", AKSNTLLower(x) title "log^6(n)", AKSNTLUpper(x) title "log^{21/2}(n)"
set output "../../Memoria/img/graphs/aks-mean.png"
plot "data/aks/direct-mean.txt" with lines title "AKS Directo", "data/aks/ntl-mean.txt" with lines title "AKS NTL"

set output "../../Memoria/img/graphs/aks-probs-primes-mean.png"
plot "data/aks-probs-primes/aks-mean.txt" with lines title "AKS", "data/aks-probs-primes/miller-rabin-mean.txt" with lines title "Miller-Rabin/40", "data/aks-probs-primes/solovay-strassen-mean.txt" with lines title "Solovay-Strassen/80"

set format x "%.2e"
set output "../../Memoria/img/graphs/aks-probs-powers-100-mean.png"
plot "data/aks-probs-powers-100/aks-mean.txt" with lines title "AKS", "data/aks-probs-powers-100/miller-rabin-mean.txt" with lines title "Miller-Rabin/40", "data/aks-probs-powers-100/solovay-strassen-mean.txt" with lines title "Solovay-Strassen/80"
set output "../../Memoria/img/graphs/aks-probs-powers-5-mean.png"
plot "data/aks-probs-powers-5/aks-mean.txt" with lines title "AKS", "data/aks-probs-powers-5/miller-rabin-mean.txt" with lines title "Miller-Rabin/40", "data/aks-probs-powers-5/solovay-strassen-mean.txt" with lines title "Solovay-Strassen/80"

set output "../../Memoria/img/graphs/aks-probs-comps-16-mean.png"
plot "data/aks-probs-comps-16/aks-mean.txt" with lines title "AKS", "data/aks-probs-comps-16/miller-rabin-mean.txt" with lines title "Miller-Rabin/40", "data/aks-probs-comps-16/solovay-strassen-mean.txt" with lines title "Solovay-Strassen/80"
set output "../../Memoria/img/graphs/aks-probs-comps-32-mean.png"
plot "data/aks-probs-comps-32/aks-mean.txt" with lines title "AKS", "data/aks-probs-comps-32/miller-rabin-mean.txt" with lines title "Miller-Rabin/40", "data/aks-probs-comps-32/solovay-strassen-mean.txt" with lines title "Solovay-Strassen/80"
