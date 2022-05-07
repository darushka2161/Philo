/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: choyda <chyoda@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 19:20:49 by choyda            #+#    #+#             */
/*   Updated: 2022/05/04 14:15:47 by choyda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	join_and_free_philos(t_info *info)
{
	int		i;

	i = 0;
	while (i < info->num_of_philo)
	{
		pthread_join(info->philos[i].thread, NULL); // ожидания завершения потока
		pthread_mutex_destroy(&info->philos[i++].check_mutex); // уничтожить мьютекс
	}
	free(info->philos);
	i = 0;
	while (i < info->num_of_philo)
		pthread_mutex_destroy(&info->forks[i++]);
	free(info->forks);
}

static void	create_philos(t_info *info)
{
	int			i;
	pthread_t	thread;

	gettimeofday(&info->create_at, NULL); //текушее время системы в качестве точки отсчета
	i = 0;
	while (i < info->num_of_philo) //проходимся по философас
	{
		info->philos[i].last_time_to_eat = info->create_at; //инициализируем время псоледнего приема пищи
		pthread_create(&info->philos[i].thread, NULL, 
			philo, &info->philos[i]); //создаем поток для философа вызывая метод  philo с параметром &info->philos[i]
		pthread_create(&thread, NULL, monitor, &info->philos[i]);//вывод сообщение о смертии philo
		pthread_detach(thread); //помечает поток, идентифицированный thread , как отсоединенный.
		++i;
	}
	if (info->num_of_must_eat != 0)
	{
		pthread_create(&thread, NULL, monitor_each_must_eat, info);//выставление значения finish 1 если все философы поели 
		pthread_detach(thread);
	}
}

int	main(int argc, char *argv[])
{
	t_info	info;
	//Мьютекс-это базовый механизм синхронизации. Он предназначен для организации взаимоисключающего доступа к общим данным для нескольких потоков с использованием барьеров памяти (для простоты можно считать мьютекс дверью, ведущей к общим данным).
	//поток — это некая независимая последовательность инструкций
	memset(&info, 0, sizeof(info)); //запосление элементов структуры 0
	if (argc != 5 && argc != 6)
		return (ft_puterror("ERROR: wrong argc\n"));
	if (init(&info, argc, argv)) //проверка ввода и создание массива философов
		return (1);
	create_philos(&info); //создание потока под каждого философа
	join_and_free_philos(&info); //удаление мьютексов
	return (0);
}
