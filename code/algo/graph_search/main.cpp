#include <iostream>
#include <vector>

using namespace std;

int main()
{
  int n, m;

  cin >> n >> m;

  vector<vector<bool>> v(n, vector<bool>(n));

  for (int i = 0; i < n; i++)
  {
    v[i][i] = true;
  }

  for (int i = 0; i < m; i++)
  {
    int v1, v2;

    cin >> v1 >> v2;

    v[v1][v2] = true;
    v[v2][v1] = true;
  }

  for (int i = 0; i < n; i++)
  {

    for (int z = 0; z < n; z++)
    {
      cout << v[i][z] << " ";
    }
    cout << endl;
  }

  return 0;
}