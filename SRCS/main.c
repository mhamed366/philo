/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkchikec <mkchikec@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:45:36 by mkchikec          #+#    #+#             */
/*   Updated: 2022/03/26 22:57:17 by mkchikec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	print(t_philo *all, char *s)
{
	pthread_mutex_lock(&(all->all->print));
	printf("%d %s", all->id, s);
	pthread_mutex_unlock(&(all->all->print));
}

int		curr_time()
{
	struct	timeval	time;
	gettimeofday(&time, NULL);
	return time.tv_sec;
}

void	eat_think_sleep(t_philo *all)
{
	int				left;
	int				right;

	left = all->id;
	right = (all->id + 1) % all->all->args.philo_count;
	pthread_mutex_lock(&(all->all->fork[left]));
	print(all, "taken 1st fork\n");
	// printf("%d taken 1st fork\n", all->id);
	pthread_mutex_lock(&(all->all->fork[right]));
	print(all, "taken 2nd fork\n");
	print(all, "taken forks\n");
	print(all, "is eating\n");
	usleep(all->all->args.eat_time * 1000);
	all->last_time_ate = curr_time();
	pthread_mutex_unlock(&(all->all->fork[left]));
	pthread_mutex_unlock(&(all->all->fork[right]));
	print(all, "is sleeping\n");
	usleep(all->all->args.sleep_time * 1000);
	print(all, "is thinking\n");
	// print(all, "gave up forks\n");
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
			if (philo->last_time_ate + philo->all->args.eat_time < curr_time())
			{
				print(philo, "DIED\n");
				exit(0);
			}
		}
	}
}

void*	process(void* arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_create(&(philo->all->death), NULL, &check_death, arg);
	while (1)
		eat_think_sleep((t_philo *)arg);
	return NULL;
}

void	run(t_all *all)
{
	t_counter	counter;

	pthread_mutex_init(&(all->print), NULL);
	counter.i = -1;
	while (++counter.i < all->args.philo_count)
	{
		pthread_mutex_init(&(all->fork[counter.i]), NULL);
		all->philos[counter.i].id = counter.i;
		all->philos[counter.i].died = 0;
		all->philos[counter.i].times_eaten = 0;
		all->philos[counter.i].all = all;
	}
	
	counter.i = -1;
	while (++counter.i < all->args.philo_count)
	{
		if (pthread_create(&(all->philo[counter.i]), NULL, &process, &(all->philos[counter.i])) != 0)
		{
			printf("ERROR\n");
			exit(0);
		}			
		usleep(100);
	}
	counter.i = -1;
	while (++counter.i < all->args.philo_count)
	{
		pthread_join(all->philo[counter.i], NULL);
	}
}

int		main(int ac, char **av)
{
	pthread_t		*philo;
	pthread_mutex_t	*fork;
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
	fork = malloc(args.philo_count * sizeof(pthread_mutex_t));
	all.philos = malloc(sizeof(t_philo) * args.philo_count);
	all.philo = philo;
	all.fork = fork;
	all.args = args;
	run(&all);
}
