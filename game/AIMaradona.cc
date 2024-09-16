#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME SinMiedo

struct PLAYER_NAME : public Player
{

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player *factory()
  {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  const vector<Dir> dirs = {Up, Down, Left, Right};
  const vector<Dir> diagonales = {DR,RU,UL,LD};
  vector<bool> soldados_mov;
    
  typedef vector<vector<int>> Tablero;
  typedef pair<Pos, int> Pos_y_Dis;


  Dir buscar_comida(Pos p, Pos comida, const Tablero &objetivo)
  {
    vector<vector<bool>> visitados(board_rows(),vector<bool>(board_cols(), false));
    queue<Pos_y_Dis> cua;
    visitados[comida.i][comida.j] = true;
    cua.push({comida, objetivo[comida.i][comida.j]});
    while(not cua.empty()){
      Pos actual = cua.front().first;
      int dis = cua.front().second;
      cua.pop();
      if(dis == 1){
        if(actual.i - p.i == -1 ) return Up;
        else if ( actual.i - p.i == 1) return Down;
        else if ( actual.j - p.j == 1) return Right;
        else if ( actual.j - p.j == -1) return Left;
      }
      for(Dir d : dirs){
        Pos sig = actual + d;
        if(pos_ok(sig) and cell(sig).type != Waste and objetivo[sig.i][sig.j] != -1 and not visitados[sig.i][sig.j]){
          visitados[sig.i][sig.j] = true;
          cua.push({sig,objetivo[sig.i][sig.j]});
        }
      }
    }
    return Up;
  };

  void comida_mas_cercana(Pos p, Pos & comida, Tablero & distancias)
  {
    vector<vector<bool>> visitados(board_rows(),vector<bool>(board_cols(),false));
    queue<Pos_y_Dis> cua;
    cua.push({p,0});
    distancias[p.i][p.j] = 0;
    visitados[p.i][p.j] = true;
    bool trobat = false;
    while(not cua.empty() and not trobat){
      Pos actual = cua.front().first;
      int dis = cua.front().second;
      cua.pop();
      for(Dir d : dirs){
        Pos sig = actual + d;
        if(pos_ok(sig) and cell(sig).type != Waste and not visitados[sig.i][sig.j]){
          cua.push({sig, dis+1});
          distancias[sig.i][sig.j] = dis+1;
          visitados[sig.i][sig.j] = true;
          if(cell(sig).food){
            comida = sig;
            return;
          }
        }
     }
    }
   }

  void comer(const vector<int> & mis_soldados)
  {
    if (mis_soldados.size() > 0){
      for(int j = 0; j < int(mis_soldados.size()); ++j ) {
        if (not soldados_mov[j]) {
          int i = mis_soldados[j];
          Unit u = unit(i);
          Pos comida;
          Tablero distancias(board_rows(), vector<int>(board_cols(),-1));
          comida_mas_cercana(u.pos, comida, distancias);
          move(i ,buscar_comida(u.pos, comida, distancias));
          soldados_mov[j] = true;
        }
      }
    }
  }


  void adjacientes(const vector<int> & mis_soldados){
    if (mis_soldados.size() > 0){
      for(int j = 0; j < int(mis_soldados.size()); ++j ) {
        if (not soldados_mov[j]) {
          int i = mis_soldados[j];
          Unit unidad = unit(i);
          Pos actual = unidad.pos;
          for(int i = 0; i < 4 and not soldados_mov[j]; ++i) {
            Dir d = dirs[i];
            Pos siguiente = actual + d;
            if(pos_ok(siguiente) and cell(siguiente).type != Waste){
              Cell celda = cell(siguiente);
              Unit u;
              if (celda.id != -1){
                u = unit(celda.id);
                if(u.player == -1) {
                  move(unidad.id, d); //caso de que haya zombie
                  soldados_mov[j] = true;
                }
                else if (u.player != me()) {
                  move(unidad.id, d); //caso de que haya jugador rival
                  soldados_mov[j] = true;
                }
              }
              else if (celda.food) {
                move(unidad.id, d);
                soldados_mov[j] = true;
              }
            }
          }
        }
      }
    }
  }



void prevision_zombies(const vector<int> & mis_soldados){
  if ( mis_soldados.size() > 0){
      for(int j = 0; j < int(mis_soldados.size()); ++j ){
       if(not soldados_mov[j]){
          int i = mis_soldados[j];
          Unit unidad = unit(i);
          Pos actual = unidad.pos;
          for(Dir d : diagonales ){
            Pos siguiente = actual + d;
            if(pos_ok(siguiente) and cell(siguiente).type != Waste){
              Cell celda = cell(siguiente);
              if(celda.id != -1){
                Unit us = unit(celda.id);
                if(us.player == -1){
                  Pos posible;
                  if(d == DR){
                    posible = actual + Up;
                    if(not soldados_mov[j] and pos_ok(posible) and cell(posible).type != Waste) {
                      move(i, Up);
                      soldados_mov[j] = true;
                    }
                    posible = actual + Left;
                    if(not soldados_mov[j] and pos_ok(posible) and cell(posible).type != Waste) { 
                      move(i, Left);
                      soldados_mov[j] = true;
                    }
                  }
                  else if ( d == LD){
                    posible = actual + Up;
                    if(not soldados_mov[j] and pos_ok(posible) and cell(posible).type != Waste) {
                      move(i, Up);
                      soldados_mov[j] = true;
                    }
                    posible = actual + Right;
                    if(not soldados_mov[j] and pos_ok(posible) and cell(posible).type != Waste) {
                      move(i, Right);
                      soldados_mov[j] = true;
                    }
                  }
                  else if (d == UL){
                    posible = actual + Down;
                    if(not soldados_mov[j] and pos_ok(posible) and cell(posible).type != Waste) {
                      move(i, Down);
                      soldados_mov[j] = true;
                    }
                    posible = actual + Right;
                    if(not soldados_mov[j] and pos_ok(posible) and cell(posible).type != Waste) {
                      move(i, Right);
                      soldados_mov[j] = true;
                    }
                  }
                  else if (d == RU){
                    Pos posible = actual + Left;
                    if(not soldados_mov[j] and pos_ok(posible) and cell(posible).type != Waste){
                      move(i, Left);
                      soldados_mov[j] = true;
                    }
                    posible = actual + Down;
                    if (not soldados_mov[j] and pos_ok(posible) and cell(posible).type != Waste) {
                      move(i, Down);
                      soldados_mov[j] = true;
                    }
                  }
                }
              }
            }
          }
        }
      }
  }
}


  /**
   * Play method, invoked once per each round.
   */
  virtual void play(){
    vector<int> mis_soldados = alive_units(me());
    soldados_mov = vector<bool>(mis_soldados.size(), false);
    adjacientes(mis_soldados);
    prevision_zombies(mis_soldados);
    comer(mis_soldados);
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
