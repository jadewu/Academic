# My ruby practice

class Sample
	def loop_practice
		# while loop
		x = 'aaa'
		while x != nil
			puts "Hello, World!"
			x = x[1..-1]
		end

		# for loop
		for i in 0..5 do
			p i
		end
		arr = ['a','b',9,8,7]
		for i in arr do
			p i
		end

		# do while loop
		t = 3
		loop do
			t -= 1
			p "Testing..."
			if t == 0
				break
			end
		end
	end

	def recursion_practice(x)
		if x == 1
			return 1
		end
		if x == 2
			return 1
		end
		return recursion_practice(x-1) + recursion_practice(x-2)
	end
end

s = Sample.new
s.loop_practice
fabonacci = []
for i in 1...10 do
	fabonacci << s.recursion_practice(i)
end
p fabonacci
