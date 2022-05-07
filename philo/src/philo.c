/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: choyda <chyoda@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 19:21:10 by choyda            #+#    #+#             */
/*   Updated: 2022/05/04 14:08:41 by choyda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	pickup_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->right);//блокировка правой вилки
	print_philo_msg(philo, "has taken a fork");
	pthread_mutex_lock(philo->left); 
	print_philo_msg(philo, "has taken a fork");
}

static void	eating(t_philo *philo)
{
	long long	ms;

	pthread_mutex_lock(&philo->check_mutex); //Если мьютекс уже заблокирован, вызывающий поток должен заблокироваться до тех пор, пока мьютекс не станет доступным.
	gettimeofday(&philo->last_time_to_eat, NULL);//обновление времени последнего приема пищи, получение времени системы
	ms = time_to_ms(philo->last_time_to_eat) - 
		time_to_ms(philo->info->create_at);//сколько прошло времени с создания до приема пищи
	pthread_mutex_lock(&philo->info->finish_mutex);
	if (!philo->info->finish)
		printf("%lld\t%d\t %s\n", ms, philo->n + 1, "is eating");
	philo->num_of_eat += 1;
	if (philo->num_of_eat == philo->info->num_of_must_eat)
		philo->info->num_of_eat_finish_philo += 1;
	pthread_mutex_unlock(&philo->info->finish_mutex);//разблокировать мьютекс и вилки снова свободны
	usleep(philo->info->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(&philo->check_mutex);
}

static void	sleeping(t_philo *philo)
{
	print_philo_msg(philo, "is sleeping");
	usleep(philo->info->time_to_sleep * 1000); // время задержки философа
}

static void	thinking(t_philo *philo)
{
	print_philo_msg(philo, "is thinking");
}

void	*philo(void *argv)
{
	t_philo	*philo;

	philo = argv;
	if (philo->n % 2 == 0) //чтобы не блокировать вилки вызываем философов через одного по очереди
		usleep(philo->info->time_to_eat * 1000);//мьютим чтобы потоки не перекрывали друг друга
	while (!philo->info->finish)
	{
		pickup_fork(philo); // блокировка вилки
		eating(philo);
		sleeping(philo);//время задержки философа
		thinking(philo);//филосов думает
	}
	return (NULL);
}
