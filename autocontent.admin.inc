<?php
/**
 * @file
 * Administrative page callbacks for the autocontent module.
 */

function autocontent_admin() {
  $rows = array();

  $config_names = config_get_names_with_prefix('autocontent.item');
  foreach ($config_names as $config_name) {
    $item = config_get($config_name);

    $node_type = node_type_load($item['node_type']);
    $author = user_load($item['node_author']);

    $links = array(
      'configure' => array(
        'title' => t('Configure'), 
        'href' => _autocontent_admin_link($item['name'], 'configure'),
      ),
      'create' => array(
        'title' => t('Create Content Now'), 
        'href' => _autocontent_admin_link($item['name'], 'create'),
      ),
      'delete' => array(
        'title' => t('Delete'), 
        'href' => _autocontent_admin_link($item['name'], 'delete'),
      ),
    );

    // Add enable or disable link.
    if ($item['enabled']) {
      $links['disable'] = array(
        'title' => t('Disable'), 
        'href' => _autocontent_admin_link($item['name'], 'disable'),
      );
    }
    else {
      $links['enable'] = array(
        'title' => t('Enable'), 
        'href' => _autocontent_admin_link($item['name'], 'enable'),
      );
    }

    $dropbuttons = array(
      '#type' => 'dropbutton',
      '#links' => $links,
    );
    
    $rows[] = array(
      'label' => check_plain($item['label']),
      'frequency' => autocontent_get_frequencies($item['frequency']),
      'interval' => autocontent_get_intervals($item['interval']),
      'node_type' => $node_type->name,
      'node_author' => user_format_name($author),
      'ops' => backdrop_render($dropbuttons),
    );
  }

  $header = array(
    t('Label'),
    t('Frequency'),
    t('Interval'),
    t('Content type'),
    t('Author'),
    t('Operations'),
  );
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Add/Configure form for configuration items.
 */
function autocontent_configure_form($form, &$form_state, $autocontent = '') {
  $author_default = '';
  $example_default = '';
  if (!empty($autocontent)) {
    $form = array('#config' => 'autocontent.item.' . $autocontent['name']);
    $author = user_load($autocontent['node_author']);
    $author_default = check_plain($author->name);
    $example_content = node_load($autocontent['node_example']);
    if ($example_content) {
      $example_default = check_plain($example_content->title);
    } 
  }
  else {
    $autocontent = array(
      'name' => '',
      'label' => '',
      'frequency' => '',
      'interval' => '',
      'node_type' => '',
      'node_author' => '',
      'node_example' => '',
    );
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => check_plain($autocontent['label']),
    '#weight' => -2,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#default_value' => $autocontent['name'],
    '#maxlength' => 32,
    '#weight' => -1,
    '#machine_name' => array(
      'exists' => 'autocontent_load',
      'source' => array('label'),
    ),
  );
  $form['frequency'] = array(
    '#type' => 'select',
    '#title' => t('Frequency'),
    '#options' => autocontent_get_frequencies(),
    '#default_value' => $autocontent['frequency'],
  );
  $form['interval'] = array(
    '#type' => 'select',
    '#title' => t('Interval'),
    '#options' => autocontent_get_intervals(),
    '#default_value' => $autocontent['interval'],
  );
  $form['node_type'] = array(
    '#type' => 'select',
    '#title' => t('Content type'),
    '#options' => node_type_get_names(),
    '#default_value' => $autocontent['node_type'],
  );
  $core_config = config('system.core');
  $form['node_author'] = array(
    '#type' => 'textfield',
    '#title' => t('Authored by'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => $author_default,
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => $core_config->getTranslated('anonymous'))),
  );

  $form['node_example'] = array(
    '#type' => 'textfield',
    '#title' => t('Example content'),
    '#autocomplete_path' => 'node/autocomplete',
    '#default_value' => $example_default,
    '#description' => t('Leave blank for empty content creation.'),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/content/autocontent',
  );

  return $form;
}

/**
 * Validate handler for autocontent_configure_form().
 
function autocontent_configure_form_validate($form, &$form_state) {
  // Validate the "Authored by" field.
  $author_name = $form_state['values']['node_author'];
  if (!empty($author_name) && !($account = user_load_by_name($author_name))) {
    // The use of empty() is mandatory in the context of usernames
    // as the empty string denotes the anonymous user. In case we
    // are dealing with an anonymous user we set the user ID to 0.
    form_set_error('name', t('The username %name does not exist.', array('%name' => $author_name)));

    //$form_state['values']['node_author'] = $account->uid;
  }

  // Validate the "Example content" field.
  $node_title = $form_state['values']['node_example'];
  if (!empty($node_title)) {
    $nid = db_select('node')
      ->fields('node', array('nid'))
      ->condition('title', db_like($node_title) . '%', 'LIKE')
      ->addTag('node_access')
      ->range(0, 1)
      ->execute()
      ->fetchField();

    //$form_state['values']['node_example'] = $nid;
  }
}
*/

/**
 * Submit handler for autocontent_configure_form().
 */
function autocontent_configure_form_submit($form, &$form_state) {
  if (isset($form['#config'])) {
    $config = config($form['#config']);
  }
  elseif (isset($form_state['values']['name'])) {
    $config = config('autocontent.item.' . $form_state['values']['name']);
  }

  $save_keys = array(
    'name',
    'label',
    'frequency',
    'interval',
    'node_type',
    'node_author',
    'node_example',
  );
  foreach ($save_keys as $key) {
    $config->set($key, $form_state['values'][$key]);
  }
  $config->set('enabled', TRUE);
  $config->save();

  // Redirect to the listing page on add.
  $form_state['redirect'] = 'admin/config/content/autocontent';
}
