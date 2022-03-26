/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkchikec <mkchikec@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:44:00 by mkchikec          #+#    #+#             */
/*   Updated: 2022/03/26 22:54:09 by mkchikec         ###   ########.fr       */
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
	long int	start_time;
}	t_args;

typedef struct s_philo {
	struct s_all		*all;
	int			id;
	long		last_time_ate;
	int			times_eaten;
	long int	start_time;
	int			died;
}	t_philo;

typedef struct s_counter {
	int	i;
	int	j;
	int	k;
}	t_counter;

typedef struct s_all {
	pthread_t		*philo;
	pthread_t		death;
	pthread_mutex_t	*fork;
	pthread_mutex_t	print;
	t_philo			*philos;
	t_args			args;
	int				curr;
}	t_all;

#endif