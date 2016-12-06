#!/usr/bin/ruby

#print ARGV.size
psOn=0
if(ARGV.size>0)
 for f in ARGV	
   if(f=="-ps")
     psOn=1
     #p psOn
   end
 end
end
foundFileName=0
if(ARGV.size>0)
  for f in ARGV	
    if(File::extname(f)==".dot")
      foundFileName=1
      #      puts "#{f}"
      pos=f.rindex(".")
#      p pos
      prefix_f=f[0..pos]
#      p prefix_f

      if(psOn==1)
        command="dot -Tps -o " + prefix_f +"ps " + f 
        p command      
        system(command)
      end
      command="dot -Tpng -o " + prefix_f +"png " + f
      p command
      system(command)
    end
  end
end
if(foundFileName==0)
  Dir::open('.') {|d|
  d.each {|f|
#    p File::extname(f)


    if(File::extname(f)==".dot")
#      puts "#{f}"
      pos=f.rindex(".")
#      p pos
      prefix_f=f[0..pos]
#      p prefix_f
      if(psOn==1)
        command="dot -Tps -o " + prefix_f +"ps " + f 
        p command      
        system(command)
      end
      command="dot -Tpng -o " + prefix_f +"png " + f
      p command
      system(command)
    end
  }
}
end


