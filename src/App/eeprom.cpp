#include "eeprom.h"



#define	AT24C16_ADDR	(uint8_t)0xA0
#define FLAG_MASK		((uint32_t)0x00FFFFFF)


I2C_HandleTypeDef	hI2C;

uint8_t				i2c_timer;


void		EEPROM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	EEPROM_CLK_ENABLE();

	hI2C.Instance = EEPROM_I2C;
	hI2C.Init.ClockSpeed = 100000;
	hI2C.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hI2C.Init.OwnAddress1 = 0;
	hI2C.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hI2C.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hI2C.Init.OwnAddress2 = 0;
	hI2C.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hI2C.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&hI2C);

	GPIO_InitStruct.Pin = EEPROM_SCL_Pin | EEPROM_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(EEPROM_GPIO_Port, &GPIO_InitStruct);

	__HAL_I2C_ENABLE(&hI2C);
	
	i2c_timer = SYSTIMER_NewCountDown(0);
}
//==============================================================================




void		EEPROM_Deinit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_I2C_DISABLE(&hI2C);
	HAL_I2C_DeInit(&hI2C);

	GPIO_InitStruct.Pin = EEPROM_SCL_Pin | EEPROM_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(EEPROM_GPIO_Port, &GPIO_InitStruct);
}
//==============================================================================




uint32_t 	_EEPROM_get_flag_status(uint32_t flag)
{
	return __HAL_I2C_GET_FLAG(&hI2C, flag);
}
//==============================================================================





uint32_t 	_EEPROM_check_errors()
{
	return (hI2C.Instance->SR1 & (_I2C_FLAG_SMBALERT | _I2C_FLAG_TIMEOUT | _I2C_FLAG_PECERR | _I2C_FLAG_OVR | _I2C_FLAG_AF | _I2C_FLAG_ARLO | _I2C_FLAG_BERR));
}
//==============================================================================





uint8_t		_EEPROM_CheckEvent(uint32_t I2C_EVENT)
{
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;

  flag1 = hI2C.Instance->SR1;
  flag2 = hI2C.Instance->SR2;
  flag2 = flag2 << 16;

  lastevent = (flag1 | flag2) & FLAG_MASK;

  if ((lastevent & I2C_EVENT) == I2C_EVENT)
  {
	  return 1;
  }
  else
  {
	  return 0;
  }
}
//==============================================================================





void		_EEPROM_Send7bitAddress(uint8_t Address, uint8_t I2C_Direction)
{
  if (I2C_Direction != 1)
  {
    /* Set the address bit0 for read */
    Address |= I2C_OAR1_ADD0;
  }
  else
  {
    /* Reset the address bit0 for write */
    Address &= (uint8_t)~((uint8_t)I2C_OAR1_ADD0);
  }
  hI2C.Instance->DR = Address;
}
//==============================================================================





void 	_EEPROM_abort()
{
	if (_EEPROM_get_flag_status(_I2C_FLAG_BUSY))
		hI2C.Instance->CR1 |= I2C_CR1_STOP;
	hI2C.Instance->CR1 = 1;
	hI2C.Instance->SR1 = hI2C.Instance->SR1 & ~(_I2C_FLAG_SMBALERT | _I2C_FLAG_TIMEOUT | _I2C_FLAG_PECERR | _I2C_FLAG_OVR | _I2C_FLAG_AF | _I2C_FLAG_ARLO | _I2C_FLAG_BERR);
}
//==============================================================================





uint8_t 	_EEPROM_generate_start()
{
	hI2C.Instance->CR1 |= I2C_CR1_START;
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT) && SYSTIMER_GetCountDown(i2c_timer))
	{
		if (_EEPROM_check_errors())
			SYSTIMER_SetCountDown(i2c_timer, 0);
	}
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	
	return 1;
}
//==============================================================================





uint8_t 	_EEPROM_generate_stop()
{
	hI2C.Instance->CR1 |= I2C_CR1_STOP;
	return 1;
}
//==============================================================================





uint8_t 	_EEPROM_send_addr_send(uint8_t addr)
{
    addr &= (uint8_t)~((uint8_t)I2C_OAR1_ADD0);
	hI2C.Instance->DR = addr;
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && SYSTIMER_GetCountDown(i2c_timer))
	{
		if (_EEPROM_check_errors())
			SYSTIMER_SetCountDown(i2c_timer, 0);
	}
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	
	return 1;
}
//==============================================================================





uint8_t 	_EEPROM_send_addr_receive(uint8_t addr)
{
    addr |= I2C_OAR1_ADD0;
	hI2C.Instance->DR = addr;
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && SYSTIMER_GetCountDown(i2c_timer))
	{
		if (_EEPROM_check_errors())
			SYSTIMER_SetCountDown(i2c_timer, 0);
	}
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	return 1;
}
//==============================================================================





uint8_t 	_EEPROM_send_memaddr_send(uint16_t addr)
{
	uint8_t addr_h = (uint8_t)((addr & 0xFF00) >> 7) & 0x0E;
	uint8_t addr_l = (uint8_t)(addr & 0x00FF);
	
	addr_h |= AT24C16_ADDR;
	hI2C.Instance->DR = addr_h;
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && SYSTIMER_GetCountDown(i2c_timer))
	{
		if (_EEPROM_check_errors())
			SYSTIMER_SetCountDown(i2c_timer, 0);
	}
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	hI2C.Instance->DR = addr_l;
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED) && SYSTIMER_GetCountDown(i2c_timer))
	{
		if (_EEPROM_check_errors())
			SYSTIMER_SetCountDown(i2c_timer, 0);
	}
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	
	return 1;
}
//==============================================================================





uint8_t 	_EEPROM_send_memaddr_receive(uint16_t addr)
{
	uint8_t addr_h = (uint8_t)((addr & 0xFF00) >> 7) & 0x0E;
	uint8_t addr_l = (uint8_t)(addr & 0x00FF);
	
	addr_h |= (AT24C16_ADDR | 0x01);
	
	hI2C.Instance->DR = addr_h;
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && SYSTIMER_GetCountDown(i2c_timer))
	{
		if (_EEPROM_check_errors())
			SYSTIMER_SetCountDown(i2c_timer, 0);
	}
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	hI2C.Instance->DR = addr_l;
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED) && SYSTIMER_GetCountDown(i2c_timer))
	{
		if (_EEPROM_check_errors())
			SYSTIMER_SetCountDown(i2c_timer, 0);
	}
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	
	return 1;
}
//==============================================================================





uint8_t 	_EEPROM_send_data(uint8_t data)
{
	hI2C.Instance->DR = data;
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED) && SYSTIMER_GetCountDown(i2c_timer))
	{
		if (_EEPROM_check_errors())
			SYSTIMER_SetCountDown(i2c_timer, 0);
	}
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	return 1;
}
//==============================================================================





// попытка чтения регистра из устройства на шине, возвращает 0 если попытка удалась или 1 если устройство не отвечает
uint8_t 	EEPROM_CheckRead(uint8_t addr, uint8_t reg, uint8_t *result)
{
	if (i2c_timer == INVALID_TIMER)
		return 0;
	
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	
	hI2C.Instance->CR1 |= I2C_CR1_ACK; // Enable I2C acknowledge
	if (!_EEPROM_generate_start())
	{
		return 0;
	}

	if (!_EEPROM_send_addr_send(addr))
	{
		return 0;
	}

	if (!_EEPROM_send_data(reg))
	{
		return 0;
	}
	
	if (!_EEPROM_generate_start())
	{
		return 0;
	}
	
	if (!_EEPROM_send_addr_receive(addr))
	{
		return 0;
	}

	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED) && SYSTIMER_GetCountDown(i2c_timer))
	{
		if (_EEPROM_check_errors())
			SYSTIMER_SetCountDown(i2c_timer, 0);
	}
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	
	uint8_t r = (uint8_t)hI2C.Instance->DR; // Receive ChipID
	if (result != 0)
		*result = r;
	hI2C.Instance->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); // Disable I2C acknowledgment
	_EEPROM_generate_stop(); // Send STOP condition
	
	return 1;
}
//==============================================================================	




uint8_t		EEPROM_ChangeSpeed(uint32_t spd)
{
	if (i2c_timer == INVALID_TIMER)
		return 0;

	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
		return 0;

	__HAL_I2C_DISABLE(&hI2C);
	hI2C.Init.ClockSpeed = spd;
	HAL_I2C_Init(&hI2C);
	__HAL_I2C_ENABLE(&hI2C);
	
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
		return 0;

	spd /= 1000 + 100;
	while(spd > 0)
		spd--;
	
	return 1;
}
//==============================================================================	




uint8_t	EEPROM_WriteReg(uint8_t addr, uint8_t reg, uint8_t value)
{
	if (i2c_timer == INVALID_TIMER)
		return 0;
	
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
		return 0;

	if (!_EEPROM_generate_start())
	{
		return 0;
	}
	
	if (!_EEPROM_send_addr_send(addr))
	{
		return 0;
	}

	
	if (!_EEPROM_send_data(reg))
	{
		return 0;
	}
	
	if (!_EEPROM_send_data(value))
	{
		return 0;
	}
	
	_EEPROM_generate_stop();
	
	return 1;
}
//==============================================================================	




uint8_t 	EEPROM_ReadReg(uint8_t addr, uint8_t reg)
{
	if (i2c_timer == INVALID_TIMER)
		return 0;
	
	uint8_t value = 0;

	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
		return 0;

	hI2C.Instance->CR1 |= I2C_CR1_ACK; // Enable I2C acknowledge
	if (!_EEPROM_generate_start())
	{
		return 0;
	}
	
	if (!_EEPROM_send_addr_send(addr))
	{
		return 0;
	}

	if (!_EEPROM_send_data(reg))
	{
		return 0;
	}
	
	if (!_EEPROM_generate_start())
	{
		return 0;
	}
	
	
	if (!_EEPROM_send_addr_receive(addr))
	{
		return 0;
	}

	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED) && SYSTIMER_GetCountDown(i2c_timer))
	{
		if (_EEPROM_check_errors())
			SYSTIMER_SetCountDown(i2c_timer, 0);
	}
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	value = (uint8_t)hI2C.Instance->DR; // Receive ChipID
	hI2C.Instance->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); // Disable I2C acknowledgment
	_EEPROM_generate_stop(); // Send STOP condition

	return value;
}
//==============================================================================	




uint8_t		EEPROM_ReadBuff(uint8_t addr, uint8_t reg, uint8_t *buff, uint16_t count)
{
	if (i2c_timer == INVALID_TIMER)
		return 0;
	
	if (count == 0)
		return 0;
	
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
		return 0;

	hI2C.Instance->CR1 |= I2C_CR1_ACK; // Enable I2C acknowledge
	if (!_EEPROM_generate_start())
	{
		return 0;
	}
	
	if (!_EEPROM_send_addr_send(addr))
	{
		return 0;
	}

	if (!_EEPROM_send_data(reg))
	{
		return 0;
	}
	
	if (!_EEPROM_generate_start())
	{
		return 0;
	}

	if (!_EEPROM_send_addr_receive(addr))
	{
		return 0;
	}

	while (count > 0)
	{
		SYSTIMER_SetCountDown(i2c_timer, 100);
		while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED) && SYSTIMER_GetCountDown(i2c_timer))
		{
			if (_EEPROM_check_errors())
				SYSTIMER_SetCountDown(i2c_timer, 0);
		}
		if (SYSTIMER_GetCountDown(i2c_timer) == 0)
		{
			_EEPROM_abort();
			return 0;
		}
		*buff = (uint8_t)hI2C.Instance->DR;
		buff++;
		count--;
	}
	
	hI2C.Instance->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); // Disable I2C acknowledgment
	_EEPROM_generate_stop(); // Send STOP condition

	return 1;
}
//==============================================================================	




uint8_t		EEPROM_WriteBuff(uint8_t addr, uint8_t reg, uint8_t *buff, uint16_t count)
{
	if (i2c_timer == INVALID_TIMER)
		return 0;
	
	if (count == 0)
		return 0;
	
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
		return 0;

	hI2C.Instance->CR1 |= I2C_CR1_ACK; // Enable I2C acknowledge
	if (!_EEPROM_generate_start())
	{
		return 0;
	}
	
	
	if (!_EEPROM_send_addr_send(addr))
	{
		return 0;
	}

	if (!_EEPROM_send_data(reg))
	{
		return 0;
	}
	
	while (count > 0)
	{
		if (!_EEPROM_send_data(*buff))
		{
			return 0;
		}
		count--;
		buff++;
	}
	
	hI2C.Instance->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); // Disable I2C acknowledgment
	_EEPROM_generate_stop();
	
	return 1;
}
//==============================================================================	




uint8_t		EEPROM_ReadMemBuff(uint16_t addr, uint8_t *buff, uint16_t count)
{
	if (i2c_timer == INVALID_TIMER)
		return 0;
	
	if (count == 0)
		return 0;
	
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}

	hI2C.Instance->CR1 |= I2C_CR1_ACK; // Enable I2C acknowledge
	if (!_EEPROM_generate_start())
	{
		_EEPROM_abort();
		return 0;
	}
	
	if (!_EEPROM_send_memaddr_send(addr))
	{
		_EEPROM_abort();
		return 0;
	}

	if (!_EEPROM_generate_start())
	{
		_EEPROM_abort();
		return 0;
	}
	if (!_EEPROM_send_addr_receive(AT24C16_ADDR))
	{
		_EEPROM_abort();
		return 0;
	}
	while (count > 0)
	{
		SYSTIMER_SetCountDown(i2c_timer, 100);
		while (!_EEPROM_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED) && SYSTIMER_GetCountDown(i2c_timer))
		{
			if (_EEPROM_check_errors())
				SYSTIMER_SetCountDown(i2c_timer, 0);
		}
		if (SYSTIMER_GetCountDown(i2c_timer) == 0)
		{
			_EEPROM_abort();
			return 0;
		}
		*buff = (uint8_t)hI2C.Instance->DR;
		buff++;
		count--;
	}
	
	hI2C.Instance->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); // Disable I2C acknowledgment
	_EEPROM_generate_stop(); // Send STOP condition
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}

	return 1;
}
//==============================================================================	




uint8_t		EEPROM_WriteMemBuff(uint16_t addr, uint8_t *buff, uint16_t count)
{
	if (i2c_timer == INVALID_TIMER)
		return 0;
	
	if (count == 0)
		return 0;

	
	hI2C.Instance->CR1 |= I2C_CR1_ACK; // Enable I2C acknowledge
	while (count > 0)
	{
		SYSTIMER_SetCountDown(i2c_timer, 100);
		while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
		if (SYSTIMER_GetCountDown(i2c_timer) == 0)
		{
			_EEPROM_abort();
			return 0;
		}

		if (!_EEPROM_generate_start())
		{
			_EEPROM_abort();
			return 0;
		}
		
		
		if (!_EEPROM_send_memaddr_send(addr))
		{
			_EEPROM_abort();
			return 0;
		}

		if (!_EEPROM_send_data(*buff))
		{
			_EEPROM_abort();
			return 0;
		}
		_EEPROM_generate_stop();
		HAL_Delay(10);
		SYSTIMER_SetCountDown(i2c_timer, 100);
		while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
		if (SYSTIMER_GetCountDown(i2c_timer) == 0)
		{
			_EEPROM_abort();
			return 0;
		}
		count--;
		buff++;
		addr++;
	}
	
	hI2C.Instance->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); // Disable I2C acknowledgment
	SYSTIMER_SetCountDown(i2c_timer, 100);
	while (_EEPROM_get_flag_status(I2C_FLAG_BUSY) && SYSTIMER_GetCountDown(i2c_timer));
	if (SYSTIMER_GetCountDown(i2c_timer) == 0)
	{
		_EEPROM_abort();
		return 0;
	}
	
	return 1;
}
//==============================================================================	




