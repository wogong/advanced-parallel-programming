! compute PI using Simpson's rule, OMP version
! date: 2016-06-04
! author: Cheng Zhen 

program compute_pi
      integer n, i
      double precision w, x, sum, pi
      double precision l_sum
      real t_start, t_end
      print *, 'Enter number of intervals:'
      read *, n

      t_start = secnds(0.0)

      w = 1.0d0/real(n) !calculate the interval size
      sum = 0.0d0

      !$OMP PARALLEL PRIVATE(x, l_sum), SHARED(w)
      l_sum = 0.0d0
      !$OMP DO
      do i = 1, n
          x = w*(real(i)-0.5d0)
          l_sum = l_sum+4.0d0/(1.0d0+x*x)
      enddo
      !$OMP ENDDO
      !$OMP CRITICAL
      sum = sum + l_sum
      !$OMP END CRITICAL
      !$OMP END PARALLEL

      pi = w*sum

      print *, 'computed pi=',pi

      t_end = secnds(t_start)
      print '("computing time = ",f10.8," seconds.")',t_end 

      stop
end program compute_pi


