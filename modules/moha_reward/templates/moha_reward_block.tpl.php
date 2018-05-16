<script src="https://www.paypalobjects.com/api/checkout.js"></script>
<script src="https://js.braintreegateway.com/web/3.33.0/js/client.min.js"></script>
<script src="https://js.braintreegateway.com/web/3.33.0/js/paypal-checkout.min.js"></script>

<div class="moha-reward-block">
  <p>Donate with PayPal, thanks.</p>
  <div>
    <input name="reward-amount" id="reward-amount-little" type="radio" value="1"><label for="reward-amount-little"> 1 USD</label>
    <input name="reward-amount" id="reward-amount-medium" type="radio" value="5" checked><label for="reward-amount-medium"> 5 USD</label>
    <input name="reward-amount" id="reward-amount-xlarge" type="radio" value="15"><label for="reward-amount-xlarge"> 15 USD</label>
  </div>
  <div id="moha-reward-braintree-button"></div>
</div>

<script>
  paypal.Button.render({
    braintree: braintree,
    client: {
      production: 'CLIENT_TOKEN_FROM_SERVER',
      // TODO: need optimized, Get value after page loaded.
      sandbox: '<?php echo $contents[MOHA_REWARD__VARIABLE__BRAINTREE_TOKEN]; ?>'
    },
    style: {
      size: 'small',
      color: 'gold',
      shape: 'pill',
      label: 'checkout'
    },
    env: '<?php echo $contents[MOHA_REWARD__VARIABLE__PAYPAL_ENVIRONMENT]; ?>', // or 'sandbox'
    payment: function(data, actions) {

      // Make a call to create the payment

      return actions.payment.create({
        payment: {
          transactions: [
            {
              amount: { total: donateAmount, currency: 'USD' }
            }
          ]
        }
      });
    },
    onAuthorize: function(data, actions) {

      // Call your server with data.nonce to finalize the payment

      console.log('Braintree nonce:', data.nonce);
      alert('Thank you for your support!');
      // Get the payment and buyer details
      // Todo: Fetch buyer details to thank later.
      return actions.payment.get().then(function(payment) {
        console.log('Payment details:', payment);
      });
    }
  }, '#moha-reward-braintree-button');

  let donateAmount=5;

  jQuery('.moha-reward-block input').change(function () {
    donateAmount = jQuery(this).val();
  })

</script>

<style>
  .moha-reward-block,
  .moha-reward-block p {
    text-align: center;
  }

  .moha-reward-block label {
    padding: 0 10px 0 5px;
  }
</style>