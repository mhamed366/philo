/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkchikec <mkchikec@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:44:00 by mkchikec          #+#    #+#             */
/*   Updated: 2022/03/28 20:43:11 by mkchikec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include "libft/libft.h"
# include <limits.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_args {
	int			philo_count;
	int			die_time;
	int			eat_time;
	int			sleep_time;
	int			eat_count;
	long		start_time;
}	t_args;

typedef struct s_philo {
	struct s_all		*all;
	int					id;
	long				last_time_ate;
	int					times_eaten;
	long				start_time;
	int					died;
	int					is_eating;
	long				started_sleeping;
	pthread_mutex_t		check;
}	t_philo;

typedef struct s_counter {
	int	i;
	int	j;
	int	k;
}	t_counter;

typedef struct s_all {
	pthread_t		*philo;
	pthread_t		death;
	pthread_t		times_ate;
	pthread_mutex_t	*fork;
	pthread_mutex_t	print;
	t_philo			*philos;
	t_args			args;
	int				curr;
}	t_all;

void	eat_think_sleep(t_philo *all);
void	*check_death(void *arg);
void	*check_times_ate(void *arg);
void	print(t_philo *all, char *s);
long	curr_time(void);
void	free_all(t_philo *philo);
#endif