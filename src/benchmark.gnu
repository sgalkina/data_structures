set terminal png enhanced size 900,640 font "arial,14"
set output 'bin/benchmark.png'
set boxwidth 0.9 absolute
set style fill   solid 1.00 border lt -1
set key inside right top vertical Right noreverse noenhanced autotitle nobox
set style histogram clustered gap 1 title textcolor lt -1
set datafile missing '-'
set style data histograms
set xtics border in scale 0,0 nomirror rotate by 0  autojustify
set xtics  norangelimit
set xtics   ()
set title "Time benchmarking for map implementations"
set xtics ("vector" 0, "hash" 1, "tree" 2)
plot 'bin/benchmark.dat' using 1:xtic(1) ti col, '' u 2 ti col, '' u 3 ti col, '' u 4 ti col, '' u 5 ti col
