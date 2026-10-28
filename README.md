# SmartWeather

Sistema de clima para SA-MP legacy com relógio, zonas climáticas, vento e mecânicas opcionais de sobrevivência.

## Requisitos

- YSI com `y_hooks`, `y_iterate` e `y_utils`

## Instalação

Copie a pasta `SmartWheater` para `pawno/include/`:

```pawn
#include <a_samp>
#include <SmartWheater/SmartWeather>
```

## Uso

```pawn
public OnGameModeInit()
{
    SW_Clock(8, 0, 1.0);
    SW_Weather(SW_WEATHER_DRY, 0);
    SW_Init();
    return 1;
}

public OnGameModeExit()
{
    SW_Exit();
    return 1;
}
```

## Funções principais

- `SW_Weather(weatherid, transition_seconds = 60)`: define o clima global.
- `SW_Season(seasonid)`: define a estação.
- `SW_Clock(hour, minute, scale = 0.0)`: define e configura o relógio.
- `SW_Zone(x, y, radius, weatherid, priority = 0)`: cria uma zona climática.
- `SW_ZoneShow(zoneid, color, squares = 12)`: exibe a zona no mapa.
- `SW_ZoneMove(zoneid, x, y, radius, weatherid = -1, color = -1)`: move ou atualiza uma zona.
- `SW_ZoneRemove(zoneid)` e `SW_ZonesClear()`: removem zonas.
- `SW_Survival(...)` e `SW_Diseases(...)`: configuram sobrevivência e doenças.
- `SW_Wind(...)` e `SW_ZoneWind(...)`: consultam o vento.

Os callbacks da include são opcionais. Implemente na gamemode somente os eventos que precisar.

`gz_shapes.inc` é incluído pelo SmartWeather e também pode ser usado para criar formas de Gang Zones.
