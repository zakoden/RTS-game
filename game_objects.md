# Игровые объекты
1. Декоративные объекты
Примеры: трава, бамбук, кусты и т.д.
Объекты без возможности столкновения или движения, нужно просто отрисовывать. 
(to do два класса static и dynamic для ускорения работы)
2. Здания 
Не нужна возможность передвижения, но должны уметь обрабатывать столкновения.
3. Юниты
Обрабатычвают столкновения как и здания, а также перемещаются.
4. Пули
Перемещаются, но обработка столкновений не нужна.

Basic types:
|                  | texture dynamic | hitbox      | movable     |
| ---------------- | --------------- | ----------- | ----------- | 
| Drawable static  | -               | -           | -           |
| Drawable dynamic | +               | -           | -           |
| Building         | +               | +           | -           |
| Unit             | +               | +           | +           |
| Bullet           | +               | -           | +           |

Therefore:  
Drawable -> Drawable static  
Drawable -> Drawable dynamic -> Building -> Unit  
Drawable -> Drawable dynamic -> Bullet   

Full table:
|                  | texture dynamic | hitbox      | movable     |
| ---------------- | --------------- | ----------- | ----------- | 
| Drawable static  | -               | -           | -           |
| Drawable dynamic | +               | -           | -           |
| Building         | -               | +           | -           |
| Bullet           | -               | -           | +           |
| Building         | +               | +           | -           |
| Unit             | -               | +           | +           |
| Bullet           | +               | -           | +           |
| Unit             | +               | +           | +           |

