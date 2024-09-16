#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Camaron

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
        if(pos_ok(sig) and objetivo[sig.i][sig.j] != -1 and not visitados[sig.i][sig.j]){
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
            trobat = true;
          }
        }
     }
    }
   }

  void comer()
  {
    vector<int> mis_soldados = alive_units(me());
    if (mis_soldados.size() > 0){
      for (int i : mis_soldados){
        Unit u = unit(i);
        Pos comida;
        Tablero distancias(board_rows(), vector<int>(board_cols(),-1));
        comida_mas_cercana(u.pos, comida, distancias);
        move(i ,buscar_comida(u.pos, comida, distancias));
      }
    }
  }


    Dir buscar_enemigo(Pos p, Pos enemigo, const Tablero &objetivo)
  {
    vector<vector<bool>> visitados(board_rows(),vector<bool>(board_cols(), false));
    queue<Pos_y_Dis> cua;
    visitados[enemigo.i][enemigo.j] = true;
    cua.push({enemigo, objetivo[enemigo.i][enemigo.j]});
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
        if(pos_ok(sig) and objetivo[sig.i][sig.j] != -1 and not visitados[sig.i][sig.j]){
          visitados[sig.i][sig.j] = true;
          cua.push({sig,objetivo[sig.i][sig.j]});
        }
      }
    }
    return Up;
  }


  void enemigo_mas_cercano(Pos p, Pos & enemigo, Tablero & distancias){
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
          int id_enemigo = cell(sig).id;
          if(id_enemigo != me() and id_enemigo != -1){
            enemigo = sig;
            trobat = true;
          }
        }
     }

    }
 };


  void matar(){
    vector<int> mis_soldados = alive_units(me());
    int mi_player = me();
    if(mis_soldados.size() > 0){
      for (int i : mis_soldados){
         Unit u = unit(i);
         Tablero distancias(board_rows(), vector<int>(board_cols(),-1));
         Pos enemigo;
         enemigo_mas_cercano(u.pos,enemigo, distancias);
         move(i, buscar_enemigo(u.pos, enemigo, distancias));
      }
    }
  };


  Dir buscar_zombie(Pos p, Pos zombie, const Tablero &objetivo){
    vector<vector<bool>> visitados(board_rows(),vector<bool>(board_cols(), false));
    queue<Pos_y_Dis> cua;
    visitados[zombie.i][zombie.j] = true;
    cua.push({zombie, objetivo[zombie.i][zombie.j]});
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
        if(pos_ok(sig) and objetivo[sig.i][sig.j] != -1 and not visitados[sig.i][sig.j]){
          visitados[sig.i][sig.j] = true;
          cua.push({sig,objetivo[sig.i][sig.j]});
        }
      }
    }
    return Up;

  }

 
void zombie_mas_cercano(Pos p, Pos & zombie, Tablero & distancias){
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
          int id_enemigo = cell(sig).id;
          Unit u = unit(id_enemigo);
          if(u.player == -1){
            zombie = sig;
            trobat = true;
          }
        }
     }

    }
 };

  void lo_que_tenga_mas_cerca(){
    vector<int> mis_soldados = alive_units(me());
    int mi_player = me();
    if(mis_soldados.size() > 0){
      for (int i : mis_soldados){
         Unit u = unit(i);

         Tablero distancias1(board_rows(), vector<int>(board_cols(),-1));
         Tablero distancias2(board_rows(), vector<int>(board_cols(),-1));
         Tablero distancias3(board_rows(), vector<int>(board_cols(),-1));

         Pos enemigo;
         Pos comida;
         Pos zombie;

         int enemigo_cercano;
         int comida_cercana;
         int zombie_cercano;

         enemigo_mas_cercano(u.pos,enemigo,distancias1);
         comida_mas_cercana(u.pos, comida, distancias2);
         zombie_mas_cercano(u.pos, zombie, distancias3);

         enemigo_cercano = distancias1[enemigo.i][enemigo.j];
         comida_cercana = distancias2[comida.i][comida.j];
         zombie_cercano = distancias3[zombie.i][zombie.j];

         int minimo = min(enemigo_cercano,min(comida_cercana,enemigo_cercano));

         if (minimo == enemigo_cercano) buscar_enemigo(u.pos, enemigo, distancias1);

         else if (minimo == comida_cercana) buscar_comida(u.pos, comida, distancias2);
         
         else if (minimo == zombie_cercano) buscar_zombie(u.pos, zombie, distancias3);

         
      }
    }
  }


  /**
   * Play method, invoked once per each round.
   */
  virtual void play(){
    lo_que_tenga_mas_cerca();
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
