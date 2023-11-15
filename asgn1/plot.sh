make monte_carlo

#
#figure 1
#

>red.dat #creates red.dat file for red points
>blue.dat #creates blue.dat file for blue points

num=1001 #sets number for monte carlo simluation
output=$(./monte_carlo -n $num | tail -n +2) #stores monte carlo into output var

red="red.dat" #stores red.dat into red var
blue="blue.dat"	#stores blue.dat into blue var

echo "$output" | awk '{if($5 == 0){print $3, $4 >> "'"$red"'"} if($5 == 1){print $3, $4 >> "'"$blue"'"}}' #pipeline from output monte carlo, if point is out circle, direct x y values to red.dat, if point is in circle, direct x y values to blue.dat

gnuplot <<END
	set terminal pdf
	set output "figure1.pdf"
	set size square 1,1
	set key off
	set title "In or Out Circle"
	set xlabel"x-values"
	set ylabel "y-values"
	set xrange [0:1]
	set yrange [0:1]
	set xtics 0.20
	set ytics 0.20
	plot "red.dat" with dots lc rgb "red", "blue.dat" with dots lc rgb "blue", [x=0:1] sqrt(1 - x**2) with lines lw 0.25 lc rgb "black" 
END

rm red.dat #removes data file for program repeat
rm blue.dat #removes data file for program repeat

#
#figure 3
#

>plot1.dat #create first line file
>plot2.dat #create second line file
>plot3.dat #create third line file
>plot4.dat #create fourth line file

num2=65536 #set number of iterations
output2=$(./monte_carlo -n $num2 | tail -n +2) #output of monte carlo
pi=3.141592653589793238 #value of pi

for((i = 1; i < 5; i++)); do #loop through each file
	output2=$(./monte_carlo -n $num2 -r $i| tail -n +2) #get output of monte carlo
	echo "$output2" | awk '{print $1, "'$pi'" - $2}' > plot$i.dat #directs iteration and error values into each plot.dat files
done

gnuplot <<END
	set terminal pdf
	set output "figure2.pdf"
	set key off
	set title "Monte Carlo Error Estimation"
	set ylabel "Error"
	set xlabel "Iterations"
	set xrange [1:65000]
	set yrange [-1:1]
	set xtics add (1, 4, 16, 64, 256, 1024, 4096, 16384)
	set ytics 0.50
	set logscale x 2
	set zeroaxis
	plot "plot1.dat" with lines, "plot2.dat" with lines, "plot3.dat" with lines, "plot4.dat" with lines
END

rm plot1.dat
rm plot2.dat
rm plot3.dat
rm plot4.dat
