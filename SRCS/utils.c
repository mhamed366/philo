/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkchikec <mkchikec@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 20:16:36 by mkchikec          #+#    #+#             */
/*   Updated: 2022/03/28 20:50:29 by mkchikec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	curr_time(void)
{
	long			res;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	res = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
	return (res);
}

void	print(t_philo *all, char *s)
{
	pthread_mutex_lock(&(all->all->print));
	printf("%ld %d %s", curr_time() - all->all->args.start_time,
		all->id + 1, s);
	pthread_mutex_unlock(&(all->all->print));
}

void	eat_think_sleep(t_philo *all)
{
	int				left;
	int				right;

	pthread_mutex_lock(&(all->check));
	left = all->id;
	right = (all->id + 1) % all->all->args.philo_count;
	pthread_mutex_lock(&(all->all->fork[left]));
	print(all, "has taken a fork\n");
	pthread_mutex_lock(&(all->all->fork[right]));
	print(all, "has taken a fork\n");
	print(all, "is eating\n");
	all->is_eating = 1;
	all->last_time_ate = curr_time();
	usleep(all->all->args.eat_time * 1000);
	all->is_eating = 0;
	all->times_eaten++;
	pthread_mutex_unlock(&(all->all->fork[left]));
	pthread_mutex_unlock(&(all->all->fork[right]));
	print(all, "is sleeping\n");
	all->started_sleeping = curr_time();
	usleep(all->all->args.sleep_time * 1000);
	print(all, "is thinking\n");
	pthread_mutex_unlock(&(all->check));
}

void	*check_death(void *arg)
{
	t_philo		*philo;
	t_counter	counter;

	philo = arg;
	while (1)
	{
		counter.i = -1;
		while (++counter.i < philo->all->args.philo_count)
		{
			if (philo->last_time_ate + philo->all->args.die_time + 1
				< curr_time() && !philo->is_eating)
			{
				print(philo, "died\n");
				free_all(philo);
				exit(0);
			}
		}
	}
}

void	*check_times_ate(void *arg)
{
	t_philo		*philo;
	t_counter	counter;

	philo = arg;
	counter.i = -1;
	while (++counter.i < philo->all->args.philo_count)
	{
		if (philo->times_eaten < philo->all->args.eat_count)
		{
			counter.i = -1;
		}
	}
	pthread_mutex_lock(&(philo->all->print));
	printf("Everyone had %d meals", philo->all->args.eat_count);
	pthread_mutex_unlock(&(philo->all->print));
	free_all(philo);
	exit(0);
}
