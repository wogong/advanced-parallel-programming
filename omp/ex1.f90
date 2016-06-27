! compute PI using Simpson's rule
! date: 2016-06-04
! author:

program compute_pi
      integer n, i
      double precision w, x, sum, pi
      real t_start, t_end
      print *, 'Enter number of intervals:'
      read *, n

      t_start = secnds(0.0)

      w = 1.0d0/real(n) !calculate the interval size
      sum = 0.0d0

      do i = 1, n
          x = w*(real(i)-0.5d0)
          sum = sum+4.0d0/(1.0d0+x*x)
      enddo

      pi = w*sum

      print *, 'computed pi=',pi

      t_end = secnds(t_start)
      print '("computing time = ",f12.8," seconds.")',t_end 

      stop
end program compute_pi
