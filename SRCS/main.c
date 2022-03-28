/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkchikec <mkchikec@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:45:36 by mkchikec          #+#    #+#             */
/*   Updated: 2022/03/28 20:51:05 by mkchikec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	free_all(t_philo *philo)
{
	t_counter	counter;

	free(philo->all->philo);
	free(philo->all->fork);
	free(philo->all->philos);
	pthread_mutex_destroy(&(philo->all->print));
	counter.i = -1;
	while (++counter.i < philo->all->args.philo_count)
	{
		pthread_mutex_destroy(&(philo->all->philos[counter.i].check));
		pthread_mutex_destroy(&(philo->all->fork[counter.i]));
	}
}

void	*process(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_create(&(philo->all->death), NULL, &check_death, arg);
	if (philo->all->args.eat_count != -1)
		pthread_create(&(philo->all->times_ate), NULL, &check_times_ate, arg);
	while (1)
		eat_think_sleep((t_philo *)arg);
	return (NULL);
}

void	run(t_all *all)
{
	t_counter	counter;

	pthread_mutex_init(&(all->print), NULL);
	counter.i = -1;
	while (++counter.i < all->args.philo_count)
	{
		pthread_mutex_init(&(all->philos[counter.i].check), NULL);
		pthread_mutex_init(&(all->fork[counter.i]), NULL);
		all->philos[counter.i].id = counter.i;
		all->philos[counter.i].died = 0;
		all->philos[counter.i].times_eaten = 0;
		all->philos[counter.i].all = all;
		all->philos[counter.i].last_time_ate = curr_time();
		all->philos[counter.i].is_eating = 0;
	}
	counter.i = -1;
	while (++counter.i < all->args.philo_count)
	{
		pthread_create(&(all->philo[counter.i]), NULL,
			&process, &(all->philos[counter.i]));
		usleep(100);
	}
	counter.i = -1;
	while (++counter.i < all->args.philo_count)
		pthread_join(all->philo[counter.i], NULL);
}

void	allocate(t_all *all, int philo_count)
{
	pthread_t		*philo;
	pthread_mutex_t	*fork;

	philo = malloc(philo_count * sizeof(pthread_t));
	fork = malloc(philo_count * sizeof(pthread_mutex_t));
	all->philos = malloc(sizeof(t_philo) * philo_count);
	all->philo = philo;
	all->fork = fork;
	if (!philo || !fork || !all->philos)
	{
		printf("Error allocating\n");
		exit(0);
	}
}

int	main(int ac, char **av)
{
	t_args			args;
	t_all			all;

	if (ac != 5 && ac != 6)
	{
		printf("Wrong number of arguments!\n");
		exit(0);
	}
	args.philo_count = ft_atoi(av[1]);
	args.die_time = ft_atoi(av[2]);
	args.eat_time = ft_atoi(av[3]);
	args.sleep_time = ft_atoi(av[4]);
	args.start_time = curr_time();
	args.eat_count = -1;
	if (ac == 6)
		args.eat_count = ft_atoi(av[5]);
	allocate(&all, args.philo_count);
	all.args = args;
	run(&all);
}
