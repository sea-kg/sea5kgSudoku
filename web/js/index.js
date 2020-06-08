function openTab(el, what) {

  // switch tabs
  var els = document.getElementsByClassName("tab-item");
  for (var i = 0; i < els.length; i++) {
    var el0 = els[i];
    el0.classList.remove("selected");
  }
  el.classList.add("selected")


  // switch container
  console.log(what)
  if (what == "generator") {
    document.getElementById("generator").style.display = "";
    document.getElementById("solver").style.display = "none";
  } else if (what == "solver") {
    document.getElementById("generator").style.display = "none";
    document.getElementById("solver").style.display = "";
  }
}

function cellId(x,y) {
  return "cellX" + x + "Y" + y;
}

function trysetvalue(value) {
  console.log(value);

  var x = window.sudokuCellSelected.x;
  var y = window.sudokuCellSelected.y;
  var elId = cellId(x,y);
  document.getElementById(elId).innerHTML = value;
}

function selectCell(el,x,y) {
  var els = document.getElementsByClassName("sudoku-cell");
  for (var i = 0; i < els.length; i++) {
    var el0 = els[i];
    el0.classList.remove("selected");
  }
  el.classList.add("selected");
  window.sudokuCellSelected = {
    x: x,
    y: y
  }
  console.log(window.sudokuCellSelected);
}

function getClassesForOutBorder(x,y,size) {
  var _classes = "";
  if (x == 0) {
    _classes += " left"
  }
  if (x == size - 1) {
    _classes += " right"
  }
  if (y == 0) {
    _classes += " top"
  }
  if (y == size - 1) {
    _classes += " bottom"
  }
  return _classes;
}

function isOneGroup(x,y, x1, y1, groups) {
  for (var i = 0; i < groups.length; i++) {
    var group = groups[i];
    var p0 = false;
    var p1 = false;
    for (var g = 0; g < group.length; g++) {
      var p = group[g];
      if (p.x == x && p.y == y) {
        p0 = true;
      }
      if (p.x == x1 && p.y == y1) {
        p1 = true;
      }
    }
    if (p0 && p1) {
      return true;
    }
  }
  return false;
}

function createSudokuField(elId, stype) {
  var size = 6;
  var alphabet = "123456";
  var groups = []
  if (stype == "5x5") {
    size = 5; 
    alphabet = "12345"
    groups = [
      [{x: 0, y: 0}, {x: 1, y: 0}, {x: 2, y: 0}, {x: 0, y: 1}, {x: 1, y: 1}],
      [{x: 3, y: 0}, {x: 4, y: 0}, {x: 3, y: 1}, {x: 4, y: 1}, {x: 4, y: 2}],
      [{x: 3, y: 3}, {x: 4, y: 3}, {x: 2, y: 4}, {x: 3, y: 4}, {x: 4, y: 4}],
      [{x: 0, y: 2}, {x: 0, y: 3}, {x: 1, y: 3}, {x: 0, y: 4}, {x: 1, y: 4}],
      [{x: 2, y: 1}, {x: 2, y: 2}, {x: 2, y: 3}, {x: 1, y: 2}, {x: 3, y: 2}],
    ]
  } else if (stype == "6x6") {
    size = 6;
    alphabet = "123456"
    groups = [
      [{x: 0, y: 0}, {x: 1, y: 0}, {x: 2, y: 0}, {x: 0, y: 1}, {x: 1, y: 1}, {x: 2, y: 1}],
      [{x: 0, y: 2}, {x: 1, y: 2}, {x: 2, y: 2}, {x: 0, y: 3}, {x: 1, y: 3}, {x: 2, y: 3}],
      [{x: 0, y: 4}, {x: 1, y: 4}, {x: 2, y: 4}, {x: 0, y: 5}, {x: 1, y: 5}, {x: 2, y: 5}],
      [{x: 3, y: 0}, {x: 4, y: 0}, {x: 5, y: 0}, {x: 3, y: 1}, {x: 4, y: 1}, {x: 5, y: 1}],
      [{x: 3, y: 2}, {x: 4, y: 2}, {x: 5, y: 2}, {x: 3, y: 3}, {x: 4, y: 3}, {x: 5, y: 3}],
      [{x: 3, y: 4}, {x: 4, y: 4}, {x: 5, y: 4}, {x: 3, y: 5}, {x: 4, y: 5}, {x: 5, y: 5}],
    ]
  } else if (stype == "9x9") {
    size = 9;
    alphabet = "123456789"
    groups = [
      [
        {x: 0, y: 0}, {x: 1, y: 0}, {x: 2, y: 0}, 
        {x: 0, y: 1}, {x: 1, y: 1}, {x: 2, y: 1},
        {x: 0, y: 2}, {x: 1, y: 2}, {x: 2, y: 2},
      ],
      [
        {x: 0, y: 3}, {x: 1, y: 3}, {x: 2, y: 3}, 
        {x: 0, y: 4}, {x: 1, y: 4}, {x: 2, y: 4},
        {x: 0, y: 5}, {x: 1, y: 5}, {x: 2, y: 5},
      ],
      [
        {x: 0, y: 6}, {x: 1, y: 6}, {x: 2, y: 6}, 
        {x: 0, y: 7}, {x: 1, y: 7}, {x: 2, y: 7},
        {x: 0, y: 8}, {x: 1, y: 8}, {x: 2, y: 8},
      ],
      [
        {x: 3, y: 0}, {x: 4, y: 0}, {x: 5, y: 0}, 
        {x: 3, y: 1}, {x: 4, y: 1}, {x: 5, y: 1},
        {x: 3, y: 2}, {x: 4, y: 2}, {x: 5, y: 2},
      ],
      [
        {x: 3, y: 3}, {x: 4, y: 3}, {x: 5, y: 3}, 
        {x: 3, y: 4}, {x: 4, y: 4}, {x: 5, y: 4},
        {x: 3, y: 5}, {x: 4, y: 5}, {x: 5, y: 5},
      ],
      [
        {x: 3, y: 6}, {x: 4, y: 6}, {x: 5, y: 6}, 
        {x: 3, y: 7}, {x: 4, y: 7}, {x: 5, y: 7},
        {x: 3, y: 8}, {x: 4, y: 8}, {x: 5, y: 8},
      ],
      [
        {x: 6, y: 0}, {x: 7, y: 0}, {x: 8, y: 0}, 
        {x: 6, y: 1}, {x: 7, y: 1}, {x: 8, y: 1},
        {x: 6, y: 2}, {x: 7, y: 2}, {x: 8, y: 2},
      ],
      [
        {x: 6, y: 3}, {x: 7, y: 3}, {x: 8, y: 3}, 
        {x: 6, y: 4}, {x: 7, y: 4}, {x: 8, y: 4},
        {x: 6, y: 5}, {x: 7, y: 5}, {x: 8, y: 5},
      ],
      [
        {x: 6, y: 6}, {x: 7, y: 6}, {x: 8, y: 6}, 
        {x: 6, y: 7}, {x: 7, y: 7}, {x: 8, y: 7},
        {x: 6, y: 8}, {x: 7, y: 8}, {x: 8, y: 8},
      ],
    ]
  }

  var el = document.getElementById(elId);
  el.innerHTML = "";
  for (var y = 0; y < size; y++) {
    for (var x = 0; x < size; x++) {
      var _classes = "sudoku-cell";
      _classes += getClassesForOutBorder(x,y,size);
      
      if (isOneGroup(x,y,x+1,y,groups)) {
        _classes += " right-onegroup";
      }
      if (isOneGroup(x,y,x-1,y,groups)) {
        _classes += " left-onegroup";
      }

      if (isOneGroup(x,y,x,y+1,groups)) {
        _classes += " bottom-onegroup";
      }

      if (isOneGroup(x,y,x,y-1,groups)) {
        _classes += " top-onegroup";
      }

      el.innerHTML += "<div class='" + _classes + "' "
        + " id='" + cellId(x,y) + "' "
        + " onclick='selectCell(this," + x + ", " + y + ")'>-</div>";
    }
    el.innerHTML += "<br>";
  }

  // variants
  el.innerHTML += "<br>";

  for (var i = 0; i < alphabet.length; i++) {
    el.innerHTML += "<div class='sudoku-variant' onclick='trysetvalue(\"" + alphabet[i] + "\")'>" + alphabet[i] + "</div>";
  }
  el.innerHTML += "<div class='sudoku-variant' onclick='trysetvalue(\"-\")'>-</div>";

  el.innerHTML += "<br>";
  el.innerHTML += "<br>";
}


function generateSudokuType(el) {
  console.log(el.value)
  createSudokuField("sudoku_field1", el.value);
}

var defaultType = document.getElementById("generator_sudoku_type").value;

createSudokuField("sudoku_field1", defaultType);