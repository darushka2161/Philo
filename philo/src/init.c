/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: choyda <chyoda@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 19:20:39 by choyda            #+#    #+#             */
/*   Updated: 2022/05/03 19:43:12 by choyda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	parsing_arg(t_info *info, int argc, char *argv[])
{
	info->num_of_philo = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		info->num_of_must_eat = ft_atoi(argv[5]);
}

static int	check_info(t_info *info, int argc)
{
	if (argc == 6 && info->num_of_must_eat <= 0)
		return (ft_puterror("ERROR: wrong num of must eat\n"));
	if (info->num_of_philo < 0)
		return (ft_puterror("ERROR: wrong num of philo\n"));
	if (info->time_to_die < 0)
		return (ft_puterror("ERROR: wrong time to die\n"));
	if (info->time_to_eat < 0)
		return (ft_puterror("ERROR: wrong time to eat\n"));
	if (info->time_to_sleep < 0)
		return (ft_puterror("ERROR: wrong time to sleep\n"));
	return (FT_SUCCESS);
}

static int	init_philos(t_info *info)
{
	int		i;

	pthread_mutex_init(&info->finish_mutex, NULL);
	if (ft_malloc(&info->philos, sizeof(t_philo) * info->num_of_philo) ||
		ft_malloc(&info->forks, sizeof(pthread_mutex_t) * info->num_of_philo)) //выделение памяти под массив философов
		return (ft_puterror("ERROR: malloc failed\n"));
	i = 0;
	while (i < info->num_of_philo)
	{
		info->philos[i].n = i; //заполняем номер философа
		pthread_mutex_init(&info->forks[i], NULL); //инициализируетмьютексы по умолчанию
		pthread_mutex_init(&info->philos[i].check_mutex, NULL); //инициализируетмьютексы по умолчанию
		if (i == 0)
			info->philos[i].left = &info->forks[info->num_of_philo - 1]; //так как первый сидит рядом с самым последним
		else
			info->philos[i].left = &info->forks[i - 1];
		info->philos[i].right = &info->forks[i];
		info->philos[i].info = info;
		++i;
	}
	return (FT_SUCCESS);
}

int	init(t_info *info, int argc, char *argv[])
{
	parsing_arg(info, argc, argv); //заполнение структуры вхдными значениями
	if (check_info(info, argc)) //вывод сообщений о невалидность ввода
		return (FT_ERROR);
	if (init_philos(info))//создан массив структур философов и инициализированны их мьютексы
		return (FT_ERROR);
	return (FT_SUCCESS);
}
