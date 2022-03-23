/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkchikec <mkchikec@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:45:36 by mkchikec          #+#    #+#             */
/*   Updated: 2022/03/23 21:52:41 by mkchikec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void*	process(void* arg)
{
	struct	timeval	time;
	int				left;
	int				right;
	t_all			*all;
	int				lock;

	all = arg;
	left = ((all->curr - 1) + all->args.philo_count) % all->args.philo_count;
	right = (all->curr + 1) % all->args.philo_count;
	gettimeofday(&time, NULL);
	while (1)
	{
		lock = 0;
		while (!lock)
		{
			pthread_mutex_lock(&(all->fork));
			if (all->count[left] || all->count[right])
			{
				pthread_mutex_unlock(&(all->fork));
				printf("error %d forks taken\n", all->curr);
			}
			all->count[left] = 1;
			all->count[right] = 1;
			pthread_mutex_unlock(&(all->fork));	
			lock = 1;			
		}
		pthread_mutex_lock(&(all->fork));
		all->count[left] = 0;
		all->count[right] = 0;
		pthread_mutex_unlock(&(all->fork));
		printf("%d taken a fork\n", all->curr);
	}
	return NULL;
}

void	run(t_all *all)
{
	t_counter	counter;

	counter.i = -1;
	
	counter.i = -1;
	while (++counter.i < all->args.philo_count)
	{
		all->curr = counter.i;
		pthread_create(&(all->philo[counter.i]), NULL, process, all);
		usleep(1);
	}
	counter.i = -1;
	while (++counter.i < all->args.philo_count)
	{
		pthread_join(all->philo[counter.i], NULL);
	}
	printf("0: %d\n",all->count[0]);
}

int		main(int ac, char **av)
{
	pthread_t		*philo;
	pthread_mutex_t	fork;
	t_args			args;
	t_all			all;
	struct			timeval	time;

	if (ac != 5 && ac != 6)
	{
		printf("Wrong number of arguments!\n");
		exit(0);
	}
	args.philo_count = ft_atoi(av[1]);
	args.die_time = ft_atoi(av[2]);
	args.eat_time = ft_atoi(av[3]);
	args.sleep_time = ft_atoi(av[4]);
	gettimeofday(&time, NULL);
	args.start_time = time.tv_sec;
	if (ac == 6)
		args.eat_count = ft_atoi(av[5]);
	else
		args.eat_count = -1;
	philo = malloc(args.philo_count * sizeof(pthread_t));
	all.count = malloc(sizeof(int) * args.philo_count);
	memset(all.count, 0, sizeof(int) * args.philo_count);	
	pthread_mutex_init(&(fork), NULL);
	all.philo = philo;
	all.fork = fork;
	all.args = args;
	run(&all);
}
