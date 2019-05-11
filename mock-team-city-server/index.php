<?php
const STATUS_SUCCESS = 'SUCCESS';
const STATUS_FAILURE = 'FAILURE';

const STAGE_RUNNING = 'running';
const STAGE_FINISHED = 'finished';

const FILE_NAME = 'status.txt';
const URL = '/httpAuth/app/rest/builds/?locator=buildType:TrafficLight_Build,state:any,count:1';
const SHOW_STATUS_URL = '/status';


$templates = [
    [
        'number' => 1,
        'status' => STATUS_SUCCESS,
        'state' => STAGE_FINISHED,
    ],
    [
        'number' => 2,
        'status' => STATUS_SUCCESS,
        'state' => STAGE_RUNNING,
    ],
    [
        'number' => 3,
        'status' => STATUS_FAILURE,
        'state' => STAGE_FINISHED,
    ],
    [
        'number' => 4,
        'status' => STATUS_FAILURE,
        'state' => STAGE_RUNNING,
    ],
    #[
    #    'number' => 5,
    #],
    #[
    #    'http-code' => 500,
    #]
];

$builds = [];

foreach ($templates as $template) {
    foreach (range(0, 3) as $item) {
        $builds[] = $template;
    }
}

function send(array $data)
{
    header("Content-Type: application/json");
    echo json_encode($data);
    return 1;
}

function get_current_number(): int
{
    return (int)@file_get_contents(FILE_NAME);
}


function get_total_count()
{
    return (int)@file_get_contents('total-count.txt');
}

function inc_total_count()
{
    $n = get_total_count();
    $f = fopen('total-count.txt', 'w');
    fwrite($f, ++$n . "\n");
    fclose($f);
}

function get_next_number(array $builds): int
{
    $number = (int)@file_get_contents(FILE_NAME);
    $f = fopen(FILE_NAME, 'w');
    $number++;
    if ($number > count($builds) - 1) {
        $number = 0;
    }
    fwrite($f, $number . "\n");
    fclose($f);

    return $number;
}

switch ($_SERVER['REQUEST_URI']) {
    case URL:

        inc_total_count();
        $number = get_next_number($builds);
        $build = $builds[$number];
        http_response_code($build['http-code'] ?? 200);

        return send([
            'number' => $number,
            'build' => [
                $builds[$number],
            ],
        ]);
        break;
    case SHOW_STATUS_URL:
        $number = get_current_number();
        $json = json_encode([
            'total-count' => get_total_count(),
            'number' => $number,
            'build' => [
                $builds[$number],
            ],
        ], JSON_PRETTY_PRINT);

        echo <<<HTML
 <html>
        <head>
<meta http-equiv="refresh" content="5"/>        
</head>
<body>
<pre>
$json
</pre>
</body>
</html>
HTML;
}




